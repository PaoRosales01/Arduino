#include <SPI.h>
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Dirección MAC de Ethernet
//192.168.0.101
IPAddress server_addr(192,168,1,203);  // IP del servidor MySQL
char user[] = "arduino";              // nombre de usuario MySQL
char password[] = "12345";        // password del usuario MySQL
// Prepara la trama para la consulta SQL
char BASE_SQL[] = "INSERT INTO basearduino.tabladatos (identifica, valor) VALUES (%s,%lu)";
char consulta[128];
EthernetClient client;  //Crea cliente Ethernet
MySQL_Connection conn((Client *)&client);  //Asocia el cliente al conector MySQL 

void setup() {  
  Serial.begin(115200);
  while (!Serial); // Espera que esté libre el puerto serie
  Ethernet.begin(mac_addr); // Inicia la conexión con el router para recibir una IP por DHCP
  Serial.print("  IP asignada por DHCP: ");
  Serial.println(Ethernet.localIP());  //Muestra la IP asignada a la shield Ethernet de Arduino
  Serial.println("Conectando...");
  if (conn.connect(server_addr, 3306, user, password)) {  
    delay(1000); //Se conecta al servidor MyQL por el puerto 3306 mediante el usuario indicado
  }
  else
    Serial.println("Connection failed.");
}

void loop() {
  delay(30000);
  Serial.println("Guardando datos");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  // Inicializa el cursor para la consulta SQL
  sprintf(consulta,BASE_SQL,"'milisegundos'",millis()); // Completa la trama de la consulta SQL
  Serial.println(consulta);
  cur_mem->execute(consulta);  // Ejecuta la consulta SQL y escribe el valor de los milisegundos
  sprintf(consulta,BASE_SQL,"'microsegundos'",micros()); // Completa la trama de la consulta SQL
  Serial.println(consulta);  
  cur_mem->execute(consulta);  // Ejecuta la consulta SQL y escribe el valor de los microsegundos
  delete cur_mem;  // Al no haber resultados, elimina el cursor y libera memoria
}
