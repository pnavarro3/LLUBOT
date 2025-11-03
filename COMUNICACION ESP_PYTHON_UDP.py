# ---- LIBRERIAS ---- #
import socket
import time

# ---- CONSTANTES ---- #
IP = "255.255.255.255"
PORT = 8888

# ---- FUNCIÓN ENVIAR AL MAESTRO: ID ROBOT DESTINO Y POSE---- #
def enviarRobot(id_robot, ang, dist, out):
    msg1 = f"id={id_robot}, ang={ang}, dist={dist}, Out={out}"
    sock.sendto(msg1.encode(), (IP, PORT))
    print(f"[ENVIADO → Robot {id_robot}] {msg1}")

# ---- FUNCIÓN RECIBIR RESPUESTA MAESTRO: ESTADO ROBOT ---- #
def recibirRespuesta():
    try:
        data, addr = sock.recvfrom(1024)
        msg2 = data.decode(errors="ignore").strip()

        if msg2.startswith("OK"):
            print(f"[RESPUESTA ← ESP] {msg2}")
    except socket.timeout:
        pass

# ---- MAIN LOOP ---- #
if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    sock.bind(("", PORT))
    sock.settimeout(0.2)

    i = 0
    while True:
        id_robot = i
        ang = 45.0
        dist = 0.1
        out = 0

        enviarRobot(id_robot, ang, dist, out)

        start = time.time()
        while time.time() - start < 0.3:
            recibirRespuesta()

        if i > 1:
            i = 0
        else:
            i += 1
        
        time.sleep(0.2)