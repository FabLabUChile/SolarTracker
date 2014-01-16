/* Setup:
          -4 Fotoresistencias conectadas en pines de la siguiente forma: 
                  -A0, up left
                  -A1, up right
                  -A2, down left
                  -A3, down right
          -2 servos, conectados en:
                  -pin 5, servo movimiento horizontal
                  -pin 6, servo movimiento vertical
   Cuando se dice "up down left right" se está asumiendo que se mira el solar tracker desde la perspectiva del sol.

*/


/******* Includes  *******/
#include <Servo.h>

/******* CONSTANTES ******/
#define H_TOLERANCE 20 // Tolerancias. Define diferencia mínima de luz que es considerada como relevante para moverse.
#define V_TOLERANCE 20 // Vertical y horizontal.
const unsigned char H_ANGLE_STEP = 1;    // Unidad mínima angular a la que se mueve el motor horizontal cada loop.
const unsigned char V_ANGLE_STEP = 1;    
const int DELAY = 20;      // Tiempo de espera hasta el siguiente loop.

/******* VARIABLES *******/
/* sensores y servos */
unsigned char sensor[]={0,1,2,3};
Servo h_servo;
Servo v_servo;
unsigned char h_angle,v_angle;

/* valores sensados y promedios para calcular en loop */
/* Los nombres de las variables son definidos según las letras iniciales de las palabras "up down left right". */
int ul_val,ur_val,dl_val,dr_val;
float left_prom, right_prom, up_prom, down_prom;

/******* MÉTODOS   *******/
void setServos(){
  h_servo.write(h_angle);
  v_servo.write(v_angle);
}

/******* MAIN      *******/
void setup(){
  /* inicializar pines */
  for (char i=0;i<4;i++){
    pinMode(sensor[i],INPUT);
  }
  h_servo.attach(5);
  v_servo.attach(6);
  
  //inicializar valores
  h_angle=90;
  v_angle=90;
  
  // debug:
  //Serial.begin(9600);
}

void loop(){
  /* leer sensores */
  ul_val=analogRead(sensor[0]);
  ur_val=analogRead(sensor[1]);
  dl_val=analogRead(sensor[2]);
  dr_val=analogRead(sensor[3]);
  
  /* sacar promedios */
  left_prom  =  (ul_val + dl_val)/2;
  right_prom =  (ur_val + dr_val)/2;
  up_prom    =  (ul_val + ur_val)/2;
  down_prom  =  (dl_val + dr_val)/2;
  
  //debug:
  /*
  Serial.print("UP_LEFT=");
  Serial.print(ul_val);
  Serial.print(" UP_RIGHT=");
  Serial.print(ur_val);
  Serial.print(" DOWN_LEFT=");
  Serial.print(dl_val);
  Serial.print(" DOWN_RIGHT=");
  Serial.println(dr_val);
  delay(500);*/
  /*
  h_servo.write(map(ul_val,0,1024,0,127));
  v_servo.write(map(ur_val,0,1024,0,127));
  delay(15);*/
  
  /* mover horizontalmente. Comparar promedios y determinar movimiento */
  if ((left_prom-right_prom>H_TOLERANCE)||(right_prom-left_prom>H_TOLERANCE))
  h_angle= (left_prom>right_prom) ? constrain((h_angle - H_ANGLE_STEP),0,179) : constrain((h_angle + H_ANGLE_STEP),0,179);
  /* mover verticalmente. Comparar y determinar movimiento */
  if ((up_prom-down_prom>V_TOLERANCE)||(down_prom-up_prom>V_TOLERANCE))
  v_angle= (up_prom>down_prom) ? constrain((v_angle - V_ANGLE_STEP),0,179) : constrain((v_angle + V_ANGLE_STEP),0,179);
  setServos();
  delay(DELAY);
}

/*
Versión 0.1
    4 fotorresistores y 2 servos.
    El Solar Tracker debe analizar las diferencias de intensidad lumínica entre los fotorresistores y según esto
    mover la cruceta. Para que se vea un movimiento más fluido, en cada iteración del ciclo mover ambos servos y 
    no solo uno. Es claro que el funcionamiento de cada servo es determinado por distintas combinaciones de 
    fotorresistores, esto es, el servo responsable de rotar en el plano, lo hace al comparar lado y lado, mientras que
    el otro servo encargado de elevar la cruceta, debe comparar luz entre los de arriba y abajo.
    Cuando se sensa un numero alto es porque se detecta mucha luz.
    
Versión 1.0
    Funcionamiento básico del solar tracker. Sigue la luz y se detiene cuando la alcanza.
    
Posibles mejoras:
    -Quinta fotorresistencia anti-dias nublados
    -Calibrar fotorresistencias según sus propias características. Quizás hacerlo calibrable con un botón, al igual que 
    el sensor de humedad.
    -
*/
