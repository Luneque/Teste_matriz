# Functional Specification Document (FSD)
## Relógio e Calendário com Matriz de LED 8x8 (TM1640) e WEMOS D1 Mini

---

### 1. Document Control
* **Title:** Functional Specification Document - Relógio e Calendário em Matriz 8x8 (WEMOS D1 Mini + TM1640)
* **Status:** Approved / In Progress
* **Target Version:** 1.0.0
* **Author:** Antigravity AI Assistant
* **Date:** 2026-10-29

---

### 2. Executive Summary & Objectives
* **High-Level Description:** O projeto consiste em um sistema de relógio e calendário IoT embarcado no microcontrolador ESP8266 (placa WEMOS D1 Mini), acoplado a um shield de matriz de LED 8x8 controlado pelo circuito integrado TM1640. O dispositivo conecta-se à rede Wi-Fi configurada, sincroniza data e hora precisas via protocolo NTP (Network Time Protocol) considerando o fuso horário de Brasília (UTC-3), e apresenta continuamente uma mensagem deslizante (*scrolling / moving message*) com a data e o horário atualizados.
* **Problem Statement:** A exibição de informações de tempo em displays miniatura de 8x8 pixels apresenta restrição severa de espaço físico (não cabem dígitos estáticos de data e hora simultaneamente). É necessária uma solução de animação contínua (marquee/scroll horizontal) com sincronização em nuvem sem a necessidade de um módulo RTC físico externo.
* **Out of Scope:**
  * Configuração de Wi-Fi via portal captivo (WiFiManager) nesta versão inicial (credenciais fornecidas estaticamente).
  * Servidor Web de configuração dinâmica de texto e brilho.
  * Suporte a múltiplos painéis em cascata.

---

### 3. User Personas & User Journeys
* **Primary Persona:** Usuário final / Observador de bancada.
* **User Journey:**
  1. O usuário energiza a placa WEMOS D1 Mini conectada à porta USB / fonte.
  2. O display exibe indicação de inicialização / conexão Wi-Fi.
  3. O ESP8266 conecta-se ao ponto de acesso Wi-Fi configurado (`S729-1`).
  4. O ESP8266 consulta servidores NTP (`pool.ntp.org` / `a.st1.ntp.br`) e atualiza o relógio interno.
  5. A matriz 8x8 passa a exibir em loop contínuo e suave a mensagem deslizante contendo a data (DD/MM/AAAA) e o horário (HH:MM:SS).
  6. Em caso de perda temporária de Wi-Fi, o ESP8266 mantém o tempo local via relógio interno e tenta reconectar em segundo plano.

---

### 4. Functional Requirements

| ID | Módulo / Componente | Descrição do Requisito | Prioridade |
| :--- | :--- | :--- | :--- |
| **FR-01** | Conectividade Wi-Fi | O sistema DEVE conectar-se automaticamente à rede SSID `S729-1` com chave `25102026`. | P0 |
| **FR-02** | Sincronização NTP | O sistema DEVE sincronizar o horário via NTP utilizando `configTime` do core ESP8266 com fuso UTC-3 (horário oficial de Brasília). | P0 |
| **FR-03** | Interface TM1640 | O sistema DEVE comunicar com o Matrix LED Shield TM1640 utilizando os pinos padrão D7 (Data/DIN) e D5 (Clock/CLK) do WEMOS D1 Mini. | P0 |
| **FR-04** | Formatação da Mensagem | A mensagem gerada para exibição DEVE conter data e hora no formato: `DD/MM/AAAA - HH:MM:SS`. | P0 |
| **FR-05** | Animação Moving Message | O sistema DEVE deslocar o texto horizontalmente pixel a pixel da direita para a esquerda em velocidade legível (aprox. 50ms a 80ms por passo). | P0 |
| **FR-06** | Atualização em Tempo Real | Ao término de cada ciclo completo de rolagem da mensagem, os valores de data e hora DEVEM ser recalculados com o tempo corrente. | P1 |
| **FR-07** | Diagnóstico Serial | O sistema DEVE imprimir logs de status da conexão Wi-Fi, IP obtido, sincronização NTP e tempo atual via porta Serial a 115200 bps. | P1 |

---

### 5. Data Flow & Interface Specifications
* **Hardware Pinout:**
  * TM1640 DIN (Data): GPIO13 (`D7`)
  * TM1640 SCLK (Clock): GPIO14 (`D5`)
  * Alimentação: 3V3 / 5V e GND
* **Estrutura de Dados e Buffer de Vídeo:**
  * `char scrollText[64]`: Armazena a string montada para exibição (`" 29/10/2026  13:55:00 "`).
  * Biblioteca gráfica `WEMOS_Matrix_GFX` (herda de `Adafruit_GFX`) gerencia o frame buffer de 8x8 pixels em `disBuffer[8]`.
* **Fluxo de Estados:**
  ```mermaid
  stateDiagram-v2
    [*] --> BOOT: Inicialização dos pinos e Display
    BOOT --> WIFI_CONNECT: Conecta à rede 'S729-1'
    WIFI_CONNECT --> NTP_SYNC: Wi-Fi Conectado
    NTP_SYNC --> UPDATE_STRING: Tempo NTP Válido
    UPDATE_STRING --> SCROLL_STEP: Renderiza frame do texto deslocado
    SCROLL_STEP --> SCROLL_STEP: Posição X > Fim do Texto
    SCROLL_STEP --> UPDATE_STRING: Ciclo de Rolagem Concluído
  ```

---

### 6. Non-Functional Requirements
* **Performance e Suavidade:** O loop de rolagem deve utilizar temporização não bloqueante baseada em `millis()` para evitar travamento do watchdog do ESP8266.
* **Consumo de Memória:** Ocupação de flash e RAM deve manter-se abaixo de 60% da capacidade do ESP8266.
* **Resiliência:** O dispositivo deve suportar oscilações de rede sem reiniciar ou congelar a exibição da matriz.

---

### 7. Acceptance Criteria
* **Cenário 1: Conexão e Sincronização**
  * **Given** que o D1 Mini foi gravado e alimentado,
  * **When** iniciar o processo de boot,
  * **Then** deve se conectar à rede Wi-Fi `S729-1`, registrar o endereço IP na Serial e sincronizar a hora via NTP.
* **Cenário 2: Exibição na Matriz 8x8**
  * **Given** que a hora foi sincronizada com sucesso,
  * **When** a rotina de exibição estiver ativa,
  * **Then** os LEDs da matriz TM1640 devem rolar suavemente a data e a hora no formato legível da direita para a esquerda.
* **Cenário 3: Validação de Hardware na COM11**
  * **Given** que a placa está plugada na porta COM11,
  * **When** o comando de gravação for disparado via `arduino-cli`,
  * **Then** o upload deve concluir com sucesso (100%) e o monitor serial deve reportar o funcionamento.
