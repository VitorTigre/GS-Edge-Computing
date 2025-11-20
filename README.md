# GS-Edge-Computing

# Descrição Geral do Projeto
Este projeto tem como objetivo monitorar níveis de foco e níveis de estresse utilizando dois potenciômetros conectados a um ESP32.
Os dados são enviados automaticamente ao ThingSpeak via protocolo HTTP, onde podem ser visualizados em tempo real através de gráficos.
Além disso, o sistema utiliza um LED RGB para indicar visualmente o nível de foco do usuário.
O projeto também contabiliza o tempo contínuo de foco e alerta o usuário quando ultrapassar 45 minutos, recomendando uma pausa mental.

# Funcionalidades
Leitura de dois potenciômetros:
PotFocus → nível de Foco (0–100%)
PotStress → nível de Estresse (0–100%)

Indicador visual com LED RGB:
Azul → foco muito baixo
Ciano → foco baixo
Amarelo → foco moderado
Verde → foco bom
Vermelho → foco muito alto (ou esforço excessivo)

Envio dos dados ao ThingSpeak a cada 15 segundos

Alerta de foco prolongado (> 45 minutos)

Conexão automática ao Wi-Fi

Totalmente compatível com Wokwi

# Componentes Utilizados
ESP32 DevKit
2 potenciômetros (10k)
LED RGB comum (cátodo comum)
Plataforma ThingSpeak

# Link para o projeto no Wokwi
 https://wokwi.com/projects/447448656438800385

# ThingSpeak – Informações do Canal
Channel ID: 3161448
Protocolo usado: HTTP

# Como funciona a comunicação com o ThingSpeak (Modo HTTP)
O ESP32 monta uma URL assim:
https://api.thingspeak.com/update
   ?api_key=CHAVE_DE_ESCRITA
   &field1=valor_do_foco
   &field2=valor_do_estresse


E envia usando:
HTTPClient http;
http.begin(url);
http.GET();
http.end();
