import socket
import threading
from threading import Lock
import time

IP = "255.255.255.255"
PORT = 8888

lock = Lock()
datos_robots = []

def recibirDatos(sock):
    while True:
        data, addr = sock.recvfrom(1024)
        msg = data.decode(errors="ignore").strip()
        
        
        try:
            partes = msg.split(',')
            robot_info = {}
            for p in partes:
                key, value = p.split('=')
                key = key.strip()
                value = value.strip()
                if key in ['id', 'Out']:
                    robot_info[key] = int(value)
                elif key in ['ang', 'dist']:
                    robot_info[key] = float(value)
            with lock:
                datos_robots.append(robot_info)
        except Exception as e:
            print(f"Error parseando mensaje: {msg} -> {e}")

def enviarDatos(sock, msg, ip, port):
    sock.sendto(msg.encode(), (ip, port))
    print(f"[Enviado -> {ip}:{port}] {msg}")

if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    sock.bind(("", PORT))

    hilo_recepcion = threading.Thread(target=recibirDatos, args=(sock,), daemon=True)
    hilo_recepcion.start()

    while True:
        with lock:
            for robot in datos_robots:
                print(f"Robot {robot['id']} -> ang: {robot['ang']}, dist: {robot['dist']}, Out: {robot['Out']}")
            datos_robots.clear()
        time.sleep(0.05)
