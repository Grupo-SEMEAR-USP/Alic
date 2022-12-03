# Alic
Projeto de aprendizagem para as crianças (ingressantes de 2021) do grupo SEMEAR/NRE da USP de São Carlos

# Compilação
Utilizar `make` para compilar o código do arduino (usando arduino-cli), `make flash` para passar dar flash por meio de avrdude (em windows, usar `make flash COM=COM<N>`, onde \<N\> é o número da porta COM do arduino utilizada, não testado)

# Setup
Para o código do computador, setup pode ser feito (idealmente em um virtual environment "venv") por meio de `pip install -r requirements.txt`.

# Rodando o projeto do computador
Para rodar o projeto, utilizar `python3 ./src/computador/Alic.py COM<N>` em windows (onde \<N\> é o número da porta COM utilizada para a conexão serial com o bluetooth), ou `python3 ./src/computador/Alic.py /dev/rfcomm<N>` em linux (onde \<N\> é o número associado a conexão serial com o bluetooth).

Para o teste utilizando turtle, utilizar `python3 ./src/computador/turtletest.py`

