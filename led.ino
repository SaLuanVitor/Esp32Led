
// Bibliotecas

#include <WiFi.h>
#include <WebServer.h>

// WiFi
const char *WIFISSID = "*****";
const char *senha = "*****";

// Mapa do Hardware

#define LED 27
#define Touch 4
String estadoLED = "Desacionado";
int mediaTouch;

// Definições de rede

IPAddress local_IP(192, 168, 1, 50);  // Defina o IP de acesso
IPAddress gateway(192, 168, 1, 1);    // Defina o IP do roteador de internet
IPAddress subnet(255, 255, 255, 0);   // Defina a máscara de sub-rede
IPAddress primaryDNS(192, 168, 1, 1); // opcional 
IPAddress secondaryDNS(8, 8, 8, 8);   // opcional 

WebServer server(80);

void setup()
{
    Serial.begin(115200);
    delay(400);

    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
    }

    Serial.println("Conectando a rede");
    Serial.println(WIFISSID);
    WiFi.begin(WIFISSID, senha);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Sucesso ao conectar-se a rede WiFi");
    Serial.print("endereço de IP para o web server: ");
    Serial.println(WiFi.localIP());
    server.on("/", handle_OnConnect);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("HTTP servidor está funcionando");
    pinMode(LED, OUTPUT);
}

void loop()
{
    server.handleClient();

    for (int i = 0; i < 100; i++)
    {
        mediaTouch += touchRead(Touch);
    }
    mediaTouch = mediaTouch / 100;

    if (mediaTouch < 40)
    {
        digitalWrite(LED, HIGH);
        estadoLED = "Acionado";
        Serial.println("Botão Acionado");
    }
    else
    {
        digitalWrite(LED, LOW);
        estadoLED = "Desacionado";
        Serial.println("Desacionado");
    }
}

void handle_OnConnect()
{
    server.send(200, "text/html", SendHTML(estadoLED));
}

void handle_NotFound()
{

    server.send(404, "text/plain", "Pagina não existente");
}
String SendHTML(String estadoLED)
{
    String ptr = "<!DOCTYPE html><html lang=\"pt-br\">\n";
    ptr += "<head><meta charset=\"UTF-8\"><meta http-equiv=\"refresh\" content=\"1\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    ptr += "<style>\n";
    ptr += "*{margin: 0; padding: 0; box-sizing: border-box;}\n";
    ptr += "header {max-width: 1900px; margin: 0 auto; background-color: black;}\n";
    ptr += ".principal {display: flex;}\n";
    ptr += ".background {background-image: url(..img/logo.jpeg); background-repeat: no-repeat; background-position: center; object-fit: cover; width: 50%; height: 100vh; background-size: cover;}\n";
    ptr += ".sensor01 {background-color: green;}\n";
    ptr += ".sensor02 {background-color: greenyellow;}\n";
    ptr += ".sensor03 {background-color: orange;}\n";
    ptr += ".tabelas {height: 200px; margin: 0 auto; margin-top: 8em; margin-right: 1em; display: flex; width: fit-content; align-items: center; background-color: gainsboro; border-radius: 5%;}\n";
    ptr += ".tabelas td {text-align: center;}\n";
    ptr += ".wifi-icon {width: 50px;}\n";
    ptr += ".wifi-icon img {width: 60px; object-fit: cover;}\n";
    ptr += ".bateria-icon {width: 60px;}\n";
    ptr += ".bateria-icon img {width: 50px; object-fit: cover;}\n";
    ptr += ".tamanho {width: 100px; font-size: 12px;}\n";
    ptr += ".sensores {width: 90px; font-size: 14px; font-weight: bolder;}\n";
    ptr += ".tabelas .sensor {width: 850px; height: 101px;}\n";
    ptr += "@media (max-width:1240px) {.principal {flex-direction: column-reverse;} .background {margin: 0 auto; width: 100%;} .tabelas {margin: 0 auto; width: 100%; height: 200px;} .tamanho {width: 50px; font-size: 10px;} .sensores {width: 90px; font-size: 12px;} .bateria {font-size: 10px;}.tabelas .sensor {width: 90%; height: 100px; margin: 0 auto;}}\n";
    ptr += "@media (max-width:500px) {.tamanho {font-size: 8px;} .bateria {font-size: 8px;}.bateria-icon img {width: 15px;} .wifi-icon {width: 1px;} .wifi-icon img {width: 20px;}}\n";
    ptr += "@media (max-width:380px) {.background {background-size: contain; background-position: top;}}\n";
    ptr += "@media (max-width:300px) {.sensores {font-size: 8px;}}\n";
    ptr += "</style>\n";
    ptr += "<link rel=\"stylesheet\" href=\"style.css\"><link rel=\"shortcut icon\" href=\"img/logo.jpeg\" type=\"image/x-icon\">\n";
    ptr += "<title>Esp32-Led</title>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<header>\n";
    ptr += "<section class=\"principal\">\n";
    ptr += "<div class=\"background\"></div>\n";
    ptr += "<div class=\"tabelas\"><table border=\"2\" class=\"sensor\">\n";
    ptr += "<tr>\n";
    ptr += "<th class=\"wifi-icon\"><img src=\"img/sinal-wifi.png\" alt=\"icone de sinal wifi\"></th>\n";
    ptr += "<th class=\"bateria-icon\"><img src=\"img/bateria-cheia.png\" alt=\"Imagem de bateriacheia\"></th>\n";
    ptr += "<th class=\"tamanho\">Nº do Dispositivo</th>\n";
    ptr += "<th class=\"tamanho\">Estado</th>\n";
    ptr += "<th class=\"tamanho\">Protegido</th>\n";
    ptr += "<th class=\"sensores\">Sensor D</th>\n";
    ptr += "<th class=\"sensores\">Sensor E</th>\n";
    ptr += "<th class=\"sensores\">Sensor M</th>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td class=\"sensor01\">&nbsp;</td>\n";
    ptr += "<td class=\"bateria01\">90%</td>\n";
    ptr += "<td>1</td>\n";
    ptr += "<td>\n";
    ptr += (String)estadoLED;
    ptr += "</td>\n";
    ptr += "<td>Sim</td>\n";
    ptr += "<td>12</td>\n";
    ptr += "<td>6</td>\n";
    ptr += "<td>3.5</td>\n";
    ptr += "</tr>\n";
    ptr += "<tr>\n";
    ptr += "<td class=\"sensor02\">&nbsp;</td>\n";
    ptr += "<td class=\"bateria02\">55%</td>\n";
    ptr += "<td>2</td>\n";
    ptr += "<td>\n";
    ptr += (String)estadoLED;
    ptr += "</td>\n";
    ptr += "<td>Sim</td>\n";
    ptr += "<td>11</td>\n";
    ptr += " <td>5</td>\n";
    ptr += " <td>2.8</td>\n";
    ptr += " </tr>\n";
    ptr += "<tr>\n";
    ptr += "<td class=\"sensor03\">&nbsp;</td>\n";
    ptr += "<td class=\"bateria03\">20%</td>\n";
    ptr += " <td>3</td>\n";
    ptr += "<td>\n";
    ptr += (String)estadoLED;
    ptr += "</td>\n";
    ptr += "<td>Sim</td>\n";
    ptr += "<td>10</td>\n";
    ptr += " <td>5.5</td>\n";
    ptr += " <td>3.3</td>\n";
    ptr += " </tr>\n";
    ptr += "</table>\n";
    ptr += "</div>\n";
    ptr += " </section>\n";
    ptr += " </header>\n";
    ptr += "</body>\n";
    ptr += "</html>\n";
    return ptr;
}
