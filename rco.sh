#!/usr/bin/env bash
#
# simpletun-setup.sh
#
# Configure tun interface + routes + forwarding and start simpletun
# for the RCO VPN practice.
#
# Usage:
#   sudo ./simpletun-setup.sh server
#   sudo ./simpletun-setup.sh client
#
# Adapt the VARIABLES section to your group/IPs before using.

set -euo pipefail

########################
# VARIABLES – EDIT ME! #
########################

# Group number (used only as documentation here)
GROUP_NUM=62

# Path to compiled simpletun binary (same on both machines)
SIMPLETUN_BIN="$HOME/simpletun/simpletun"

# TUN network /30 used for the tunnel (10.125.1.0/30 in the example)
TUN_NET_BASE="10.125.62"
SERVER_TUN_IP="${TUN_NET_BASE}.1"
CLIENT_TUN_IP="${TUN_NET_BASE}.2"

# LAN networks behind each RCO (site-to-site)
SERVER_LAN_NET="10.25.1.0/24"   # LAN behind RCO-noX
CLIENT_LAN_NET="10.55.1.0/24"   # LAN behind RCO-X

# Real LAN IPs of each RCO (used as src in routes, and gw on routers)
SERVER_LAN_IP="10.25.1.2"       # ens37 of RCO-noX
CLIENT_LAN_IP="10.55.1.137"     # ens37 of RCO-X  (adapt to your VM!)

# Public / WAN IP of ddwrt-noX that the client will connect to
SERVER_PUBLIC_IP="192.168.239.128"

# Tun device names as used in the practice
SERVER_TUN_DEV="tun0"
CLIENT_TUN_DEV="tun3"

#################################
# DO NOT EDIT BELOW THIS LINE   #
#################################

ROLE="${1:-}"

if [[ "$EUID" -ne 0 ]]; then
  echo "[-] This script must be run as root (use sudo)." >&2
  exit 1
fi

if [[ -z "$ROLE" ]]; then
  echo "Usage: $0 {server|client}" >&2
  exit 1
fi

if [[ ! -x "$SIMPLETUN_BIN" ]]; then
  echo "[-] simpletun binary not found or not executable at: $SIMPLETUN_BIN" >&2
  echo "    Make sure you compiled it (see practice instructions)." >&2
  exit 1
fi

enable_forwarding() {
  echo "[*] Enabling IPv4 forwarding (temporary, until reboot)..."
  sysctl -w net.ipv4.ip_forward=1 >/dev/null
}

setup_server() {
  echo "[*] Configuring SERVER (RCO-noX)..."

  # 1. Create tun device and assign IP
  echo "  - Creating $SERVER_TUN_DEV and assigning $SERVER_TUN_IP/30"
  ip tuntap add dev "$SERVER_TUN_DEV" mode tun || true
  ip link set "$SERVER_TUN_DEV" up
  ip addr flush dev "$SERVER_TUN_DEV" || true
  ip addr add "${SERVER_TUN_IP}/30" dev "$SERVER_TUN_DEV"

  # 2. Start simpletun in server mode
  echo "  - Starting simpletun in SERVER mode on $SERVER_TUN_DEV (port 55555)"
  # Kill any previous instance using this tun, just in case
  pkill -f "$SIMPLETUN_BIN.*-i $SERVER_TUN_DEV" 2>/dev/null || true
  "$SIMPLETUN_BIN" -i "$SERVER_TUN_DEV" -s >/var/log/simpletun-server.log 2>&1 &

  # 3. Add route to client LAN over the tunnel (site-to-site)
  echo "  - Adding route to CLIENT LAN $CLIENT_LAN_NET via $SERVER_TUN_DEV src $SERVER_LAN_IP"
  ip route replace "$CLIENT_LAN_NET" dev "$SERVER_TUN_DEV" src "$SERVER_LAN_IP"

  # 4. Enable forwarding
  enable_forwarding

  echo "[+] SERVER configuration finished."
}

setup_client() {
  echo "[*] Configuring CLIENT (RCO-X)..."

  # 1. Create tun device and assign IP
  echo "  - Creating $CLIENT_TUN_DEV and assigning $CLIENT_TUN_IP/30"
  ip tuntap add dev "$CLIENT_TUN_DEV" mode tun || true
  ip link set "$CLIENT_TUN_DEV" up
  ip addr flush dev "$CLIENT_TUN_DEV" || true
  ip addr add "${CLIENT_TUN_IP}/30" dev "$CLIENT_TUN_DEV"

  # 2. Start simpletun in client mode, connecting to SERVER_PUBLIC_IP
  echo "  - Starting simpletun in CLIENT mode on $CLIENT_TUN_DEV connecting to $SERVER_PUBLIC_IP"
  # Kill any previous instance using this tun, just in case
  pkill -f "$SIMPLETUN_BIN.*-i $CLIENT_TUN_DEV" 2>/dev/null || true
  "$SIMPLETUN_BIN" -i "$CLIENT_TUN_DEV" -c "$SERVER_PUBLIC_IP" >/var/log/simpletun-client.log 2>&1 &

  # 3. Add route to server LAN over the tunnel (site-to-site)
  echo "  - Adding route to SERVER LAN $SERVER_LAN_NET via $CLIENT_TUN_DEV src $CLIENT_LAN_IP"
  ip route replace "$SERVER_LAN_NET" dev "$CLIENT_TUN_DEV" src "$CLIENT_LAN_IP"

  # 4. Enable forwarding
  enable_forwarding

  echo "[+] CLIENT configuration finished."
}

case "$ROLE" in
  server)
    setup_server
    ;;
  client)
    setup_client
    ;;
  *)
    echo "Usage: $0 {server|client}" >&2
    exit 1
    ;;
esac
