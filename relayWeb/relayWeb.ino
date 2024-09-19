#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "IoTB"; // Nombre de la red WiFi
const char* password = "inventaronelVAR"; // Contraseña del WiFi

WebServer server(80); // Inicializar el servidor web en el puerto 80

const int relayPin = 12;
const int relayPin2 = 13; // Pin donde está conectado el relé (cambia según tu conexión)

bool relayState = false; // Estado del relé

// Página web que será servida al cliente
String getHTML() {
  String html = "<html><head><title>Control Relay</title></head>";
  html += "<body><h1>Controla el LED del Relay</h1>";
  html += relayState ? "<p>Estado: <strong>ENCENDIDO</strong></p>" : "<p>Estado: <strong>APAGADO</strong></p>";
  html += "<button onclick=\"window.location.href='/toggle'\">Cambiar estado</button>";
  html += "</body></html>";
  return html;
}

// Función para manejar el cambio de estado del relé
void handleToggle() {
  relayState = !relayState; // Cambiar el estado del relé
  digitalWrite(relayPin, relayState ? HIGH : LOW);
    digitalWrite(relayPin2, relayState ? LOW : HIGH); // Encender o apagar el relé
  server.send(200, "text/html", getHTML()); // Enviar la página web actualizada
}

// Función para manejar la página principal
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
    pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin2, LOW); // Asegurarse que el relé empieza apagado

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Conectando a ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado al WiFi.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot); // Llamar a handleRoot en la raíz "/"
  server.on("/toggle", handleToggle); // Llamar a handleToggle en "/toggle"

  server.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  server.handleClient(); // Manejar los clientes que se conectan
}
