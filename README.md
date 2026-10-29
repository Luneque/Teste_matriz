# Teste Matriz 8x8 - Relógio & Calendário NTP (WEMOS D1 Mini + TM1640)

Projeto desenvolvido para a placa **WEMOS D1 Mini (ESP8266MOD)** acoplada ao **Matrix LED Shield 8x8 (TM1640)**, exibindo data e hora sincronizadas via internet (protocolo NTP) em formato de mensagem deslizante (*moving message* / marquee contínuo).

---

## 📌 Funcionalidades

- **Conexão Wi-Fi automática**: Estabelece conexão direta com a rede configurada.
- **Sincronização NTP**: Obtém data e hora precisas via servidores NTP (`pool.ntp.org` / `a.st1.ntp.br`).
- **Fuso Horário configurado**: Horário de Brasília (UTC-3).
- **Moving Message contínua**: Animação suave deslizando da direita para a esquerda na matriz de 8x8 LEDs.
- **Formato da mensagem**: `DD/MM/AAAA  HH:MM:SS`.
- **Atualização Dinâmica**: A cada ciclo completo de rolagem da mensagem, o relógio é atualizado com o horário corrente.
- **Logs de Diagnóstico**: Saída serial a 115200 bps com status da rede, IP e dados de depuração.

---

## 🛠️ Hardware Utilizado

| Componente | Descrição / Modelo |
| :--- | :--- |
| **Microcontrolador** | WEMOS D1 Mini (ESP8266EX) |
| **Display** | Matrix LED Shield 8x8 (Driver TM1640) |
| **Pinos de Comunicação** | DIN (Data) -> `D7` (GPIO13) <br> SCLK (Clock) -> `D5` (GPIO14) |
| **Alimentação** | 3.3V / 5V e GND via pinos do Shield |

---

## 📦 Bibliotecas Necessárias

- **[Wemos Matrix Adafruit GFX](https://github.com/thomasfredericks/wemos_matrix_gfx)** (v1.4.0 ou superior)
- **[Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)**
- **ESP8266 Core para Arduino** (v3.1.2 ou superior)

---

## 🚀 Como Compilar e Gravar

### Usando o Arduino IDE ou Arduino CLI:

1. Clone o repositório:
   ```bash
   git clone git@github.com:Luneque/Teste_matriz.git
   cd Teste_matriz
   ```

2. Instale as dependências de biblioteca:
   ```bash
   arduino-cli lib install "Wemos Matrix Adafruit GFX"
   ```

3. Compile e envie para a placa (ajuste a porta COM conforme seu sistema):
   ```bash
   # Compilar
   arduino-cli compile --fqbn esp8266:esp8266:d1_mini Teste_matriz.ino

   # Gravar (ex: COM11)
   arduino-cli upload -p COM11 --fqbn esp8266:esp8266:d1_mini Teste_matriz.ino
   ```

4. Monitore a porta Serial a **115200 baud** para acompanhar o boot e a conexão Wi-Fi.

---

## 📄 Documentação

A especificação funcional completa e estruturada deste projeto está documentada em:
- [FSD - Functional Specification Document](docs/FSD.md)
