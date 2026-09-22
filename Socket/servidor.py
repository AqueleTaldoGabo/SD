from socket import socket, AF_INET, SOCK_STREAM

class JogoDaVelha:
    def __init__(self):
        self.matriz = [[" " for _ in range(3)] for _ in range(3)]

    def exibir_tabuleiro(self):
        texto = "\n  0   1   2\n"
        for i in range(3):
            texto += f"{i} {self.matriz[i][0]} | {self.matriz[i][1]} | {self.matriz[i][2]}\n"
            if i < 2:
                texto += "  ---------\n"
        return texto

    def fazer_jogada(self, posicao_str, simbolo):
        linha, coluna = map(int, posicao_str.split(","))
        
        if self.matriz[linha][coluna] == " ":
            self.matriz[linha][coluna] = simbolo
            return True
        return False

    def verificar_vencedor(self):
        m = self.matriz
        for i in range(3):
            if m[i][0] == m[i][1] == m[i][2] != " ":
                return m[i][0]
            if m[0][i] == m[1][i] == m[2][i] != " ":
                return m[0][i]

        if m[0][0] == m[1][1] == m[2][2] != " ":
            return m[0][0]
        if m[0][2] == m[1][1] == m[2][0] != " ":
            return m[0][2]

        cheio = all(m[i][j] != " " for i in range(3) for j in range(3))
        if cheio:
            return "Empate"

        return None  

class ServidorSocket:
    def run(self):
        jogo = JogoDaVelha()
        s = socket(AF_INET, SOCK_STREAM)
        s.bind(('0.0.0.0', 5001)) 
        s.listen(2)
        
        print("Aguardando Jogador 1...")
        conn1, addr1 = s.accept()
        conn1.send("Você é o Jogador X! Aguardando o Jogador 2 conectar...\n".encode())
        
        print("Aguardando Jogador 2...")
        conn2, addr2 = s.accept()
        conn2.send("Você é o Jogador O! O jogo vai começar!\n".encode())

        jogadores = [
            {"conn": conn1, "simbolo": "X"},
            {"conn": conn2, "simbolo": "O"}
        ]
        
        turno = 0 

        while True:
            atual = jogadores[turno]
            outro = jogadores[(turno + 1) % 2]

            atual["conn"].send((jogo.exibir_tabuleiro() + "\nSua vez! Digite linha,coluna: ").encode())
            outro["conn"].send((jogo.exibir_tabuleiro() + "\nAguardando a jogada do oponente...\n").encode())

            data = atual["conn"].recv(1024).decode().strip()
            if not data:
                break

            if not jogo.fazer_jogada(data, atual["simbolo"]):
                atual["conn"].send("Jogada inválida! Tente novamente.\n".encode())
                continue 

            vencedor = jogo.verificar_vencedor()
            if vencedor:
                msg_final = jogo.exibir_tabuleiro() + f"\nFIM DE JOGO! Resultado: {vencedor}\n"
                conn1.send(msg_final.encode())
                conn2.send(msg_final.encode())
                break

            turno = (turno + 1) % 2

        conn1.close()
        conn2.close()
        s.close()

if __name__ == "__main__":
    servidor = ServidorSocket()
    servidor.run()