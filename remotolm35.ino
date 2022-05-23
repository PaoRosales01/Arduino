#include <SPI.h>
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <Wire.h> 

int t = A0;                                 
unsigned long lectura;                               
unsigned long c;    

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Dirección MAC de Ethernet
//192.168.0.101
IPAddress server_addr(92,249,45,155);  // IP del servidor MySQL //ip local 192,168,0,5 //ip hostinger 92,249,45,155
char user[] = "u646610080_rosalespaola";              // nombre de usuario MySQL //usuario local arduino 
char password[] = "Utd_2020";        // password del usuario MySQL //contrase local 12345
// Prepara la trama para la consulta SQL
char BASE_SQL[] = "INSERT INTO u646610080_rosalespaola.temperaturas (sensor, valor) VALUES (%s,%lu)";
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
  lectura = analogRead (t);         
  c = (580 * lectura) / 1023;   
  Serial.println(c);
  delay(5000);
  Serial.println("Guardando datos");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);  // Inicializa el cursor para la consulta SQL
  sprintf(consulta,BASE_SQL,"'LM35'",c); // Completa la trama de la consulta SQL
  Serial.println(consulta);
  cur_mem->execute(consulta);  // Ejecuta la consulta SQL y escribe el valor de los milisegundos
  delete cur_mem;  // Al no haber resultados, elimina el cursor y libera memoria
}
