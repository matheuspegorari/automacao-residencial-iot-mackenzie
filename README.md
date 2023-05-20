# Automação Residencial com RFID e Controle de Temperatura e Umidade

Este projeto consiste em um sistema de automação residencial que utiliza um módulo RFID como gatilho para abrir a porta. Após a leitura do cartão, são coletadas informações de temperatura e umidade, e com base em limites pré-definidos, é possível controlar dispositivos eletrônicos, como ventiladores, ar-condicionados e umidificadores.

## Funcionamento e Uso

1. O módulo RFID é responsável pela leitura do cartão de acesso.
2. Após a leitura do cartão, a porta é aberta.
3. São coletadas informações de temperatura e umidade por meio de um sensor.
4. Com base em thresholds (limiares) previamente configurados, o sistema decide quais dispositivos eletrônicos devem ser ligados.
5. Os dispositivos eletrônicos, como ventiladores, ar-condicionados ou umidificadores, são controlados através de um atuador (relé).
6. Os dados de temperatura e umidade são registrados em um servidor online, utilizando o protocolo MQTT e o serviço ThingSpeak.

## Software Desenvolvido e Documentação de Código

O software desenvolvido consiste em uma integração entre o módulo RFID, o sensor de temperatura e umidade e o atuador (relé), utilizando a plataforma de desenvolvimento Arduino IDE.

A documentação detalhada do código pode ser encontrada no diretório [código](./codigo), onde estão disponíveis os arquivos-fonte e instruções de como configurar o ambiente de desenvolvimento.

## Hardware Utilizado

Os componentes de hardware utilizados neste projeto são os seguintes:

- Plataforma de desenvolvimento: ESP32
- Sensores de temperatura e umidade
- Atuador: Relé

Para a montagem do projeto, podem ser necessárias medidas e peças impressas em 3D, dependendo das especificações do ambiente onde será instalado. No entanto, esses detalhes não foram fornecidos nesta documentação.

## Interfaces, Protocolos e Módulos de Comunicação

O projeto utiliza os seguintes protocolos de comunicação e módulos:

- **Serial**: Utilizado para a comunicação entre o Arduino e o computador, permitindo a depuração do código e a exibição de mensagens no monitor serial.

- **WiFi**: Utilizado para conectar o Arduino a uma rede sem fio, possibilitando a comunicação com a internet e serviços remotos.

- **MQTT (Message Queuing Telemetry Transport)**: Utilizado para a comunicação entre o Arduino e o serviço ThingSpeak. O protocolo MQTT é empregado para enviar os dados de temperatura e umidade para o ThingSpeak, onde são armazenados e podem ser analisados. A biblioteca "PubSubClient.h" é utilizada para implementar a comunicação MQTT no projeto.

Além dos protocolos mencionados acima, também são utilizados módulos e bibliotecas específicas para cada sensor e atuador:

- **Adafruit_PN532**: Biblioteca utilizada para a comunicação com o leitor NFC PN532, permitindo a leitura de cartões RFID.

Esses protocolos e módulos são fundamentais para o funcionamento do projeto, possibilitando a coleta de dados dos sensores, o controle dos atuadores e a comunicação com serviços externos. Cada protocolo possui uma função específica e é utilizado de acordo com os requisitos do projeto.

## Licença

Este projeto está licenciado sob a MIT License.
