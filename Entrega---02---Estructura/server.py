import socket
import json

# Configuración de red: '0.0.0.0' permite escuchar conexiones desde cualquier IP de la red local
HOST = '0.0.0.0'
PORT = 65432

def iniciar_servidor():
    servidor = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    servidor.bind((HOST, PORT))
    servidor.listen()
    
    print("=" * 50)
    print("¡SUPERRRR! Servidor de Parking Python iniciado...")
    print(f"Escuchando en el puerto {PORT}...")
    print("=" * 50)
    
    # Conjunto (set) para almacenar las placas que están actualmente dentro del parking
    autos_en_parking = set()

    while True:
        conexion, direccion = servidor.accept()
        print(f"\n[+] Conexión establecida desde la IP: {direccion[0]}")
        
        try:
            # Recibimos la placa enviada por el cliente C++
            datos = conexion.recv(1024).decode('utf-8')
            if not datos:
                continue
            
            placa = datos.strip().upper()
            print(f"-> Placa procesada: {placa}")

            # Lógica de Entrada o Salida automática
            if placa in autos_en_parking:
                autos_en_parking.remove(placa)
                mensaje = f"¡SALIDA registrada con éxito para la placa {placa}!"
            else:
                autos_en_parking.add(placa)
                mensaje = f"¡ENTRADA registrada con éxito para la placa {placa}!"

            total_actual = len(autos_en_parking)
            print(f"[*] Estado actual -> Autos en el parking: {total_actual}")

            # Preparamos la respuesta en formato JSON para el cliente
            respuesta = {
                "status": "ok",
                "placa": placa,
                "total_en_parking": total_actual,
                "mensaje": mensaje
            }
            
            conexion.sendall(json.dumps(respuesta).encode('utf-8'))
            
        except Exception as e:
            print(f"¡Atención, error en la conexión!: {e}")
        finally:
            conexion.close()

if __name__ == "__main__":
    iniciar_servidor()