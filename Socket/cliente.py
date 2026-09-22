from socket import socket, AF_INET, SOCK_STREAM

class ClienteSocket:
    def run(self):
        s = socket(AF_INET, SOCK_STREAM)
        HOST = "192.168.156.10"
        PORT = 5001
        
        s.connect((HOST, PORT))

        while True:
            mensagem = s.recv(1024).decode()
            if not mensagem:
                break
            
            print(mensagem)

            if "FIM DE JOGO" in mensagem:
                break

            if "Sua vez!" in mensagem or "inválida" in mensagem:
                jogada = input("> ")
                s.send(jogada.encode())

        s.close()

if __name__ == "__main__":
    cliente = ClienteSocket()
    cliente.run()