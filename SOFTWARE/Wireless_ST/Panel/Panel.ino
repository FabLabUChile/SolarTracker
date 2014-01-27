/* Setup:
          -Arduino FIO alimentado por usb desde LiPo Rider
          -Servos conectados en D5 y D6. Horizontal y Vertical respectivamente
*/
#include <Servo.h>

const int WAIT_TIME=1000;// Si detecta h o v, espera WAIT_TIME (en ms) a que le entreguen un valor.
const char SERVO_STEP=1;
const int SERVO_DELAY=25;
unsigned char TIMEOUT;  //1 si se llego a WAIT_TIME. 0 si no.
long t0,t1;
int h_angle=0;
int v_angle=0;

Servo h_servo;
Servo v_servo;

unsigned char esperar(){
  t0=millis();
  while (Serial.available()==0){
    t1=millis();
    if (t1-t0>WAIT_TIME){
      //borrar buffer serial
      while (Serial.available())
        Serial.read();
      //avisar que se llego a timeout
      return 1;
    }
  }
  return 0;
}

void setPosition(){
  unsigned char s=Serial.read();
  switch (s){
    case 'h':
      TIMEOUT=esperar();
      if (TIMEOUT){
        Serial.println("TO");
        break;
      }
      h_angle=Serial.read();
      h_angle=constrain(h_angle,0,179);//constrain(Serial.read(),0,179);
      Serial.print("H ");
      Serial.println(h_angle);
      break;
    case 'v':
      TIMEOUT=esperar();
      if (TIMEOUT){
        Serial.println("TO");
        break;
      }
      v_angle=Serial.read();
      v_angle=constrain(v_angle,0,179);//constrain(Serial.read(),0,179);
      delay(20);
      Serial.print("V ");
      Serial.println(v_angle);
      moveServos();
      break;
    default:
      Serial.println("E");
  }
}
// controla con qué velocidad se mueven los servos. La función no termina hasta que los servos se ubican en su lugar.
void moveServos(){
  int h_read=h_servo.read();
  int v_read=v_servo.read();
  while ((h_angle!=h_read)||(v_angle!=v_read)){
    if (h_read!=h_angle)
      h_read = (h_read>h_angle) ? ((h_read-h_angle < SERVO_STEP)? h_angle : (h_read-SERVO_STEP) ) : ((h_angle-h_read<SERVO_STEP)? h_angle : (h_read+SERVO_STEP));
    if (v_read!=v_angle)
      v_read = (v_read>v_angle) ? ((v_read-v_angle < SERVO_STEP)? v_angle : (v_read-SERVO_STEP) ) : ((v_angle-v_read<SERVO_STEP)? v_angle : (v_read+SERVO_STEP));
    h_servo.write(h_read);
    v_servo.write(v_read);
    delay(SERVO_DELAY);
  }
  //limpiar buffer serial
  while (Serial.available())
    Serial.read();
}

void setup(){
  /* inicializa pines*/
  h_servo.attach(5);
  v_servo.attach(6);
  
  Serial.begin(9600);
}

void loop(){
  if (Serial.available()){
    /* recibe datos */
    setPosition();
  }
}


/*

*/

