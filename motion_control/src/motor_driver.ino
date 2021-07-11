
#include <Encoder.h>
#include <RobojaxBTS7960.h>

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>


//------right motor----------
#define RPWM 5 // define pin 3 for RPWM pin (output)
#define R_EN 4 // define pin 2 for R_EN pin (input)
#define R_IS 15 // define pin 5 for R_IS pin (output)

#define LPWM 6 // define pin 6 for LPWM pin (output)
#define L_EN 7 // define pin 7 for L_EN pin (input)
#define L_IS 13 // define pin 8 for L_IS pin (output)
#define CW 0 //do not change
#define CCW 1 //do not change
#define debug 0//change to 0 to hide serial monitor debugging infornmation or set to 1 to view


//--------left motor-----------
#define RPWM2 8 // define pin 3 for RPWM pin (output)
#define R_EN2 10 // define pin 2 for R_EN pin (input)
#define R_IS2 14 // define pin 5 for R_IS pin (output)

#define LPWM2 9 // define pin 6 for LPWM pin (output)
#define L_EN2 11 // define pin 7 for L_EN pin (input)
#define L_IS2 12 // define pin 8 for L_IS pin (output)
#define CW 0 //do not change
#define CCW 1 //do not change
#define debug 0//change to 0 to hide serial monitor debugging infornmation or set to 1 to view


RobojaxBTS7960 motor2(R_EN,RPWM,R_IS, L_EN,LPWM,L_IS,debug);


RobojaxBTS7960 motor(R_EN2,RPWM2,R_IS2, L_EN2,LPWM2,L_IS2,debug);


Encoder knobRight(3, 2);
Encoder knobLeft(19, 18);


ros::NodeHandle  nh;
long EncoderRead=0;
long EncoderRead2=0;

long positionLeft  = -99999;
long positionRight = -99999;



//publishing
std_msgs::Int16 str_msg;
ros::Publisher chatter("lwheel_ticks", &str_msg);

std_msgs::Int16 str_msg2;
ros::Publisher chatter2("rwheel_ticks", &str_msg2);

//char hello[25] = "publishing on ros!";

//subscrbing
void messageCb (const std_msgs::Float32& Vmsg){
       int motor_value= int(Vmsg.data);
      
       if(motor_value>0 && motor_value<=255)
       {
         int x = map(motor_value, 0,255,0,100);
         motor.rotate(x,CW);
//         delay(50);
        }else if(motor_value<0 && motor_value>-255)
        { 
         int y = map(abs(motor_value),0, 255,0,100);
         motor.rotate(y,CCW);
//         delay(50);
          }else
          {
            motor.stop();
            }

}

//-----------------------------------
void messageCb2 (const std_msgs::Float32& Vmsg){
       int motor_value2= int(Vmsg.data);
      
       if(motor_value2>0 && motor_value2<=255)
       {
         int x2 = map(motor_value2, 0,255,0,100);
         motor2.rotate(x2,CW);
//         delay(50);
        }else if(motor_value2<0 && motor_value2>-255)
        { 
         int y2 = map(abs(motor_value2),0, 255,0,100);
         motor2.rotate(y2,CCW);
//         delay(50);
          }else
          {
            motor2.stop();
            }

}

ros::Subscriber<std_msgs::Float32> sub("lmotor", messageCb);
ros::Subscriber<std_msgs::Float32> sub2("rmotor", messageCb2);


void setup()
{
  Serial.begin(57600);// setup Serial Monitor to display information

  nh.initNode();
  motor.begin();
  motor2.begin();

//
  nh.advertise(chatter);
  nh.advertise(chatter2);

  nh.subscribe(sub);
  nh.subscribe(sub2);


}

void loop()
{
  EncoderRead= Encoder_Read();
  EncoderRead2= Encoder_Read2();
  str_msg.data = EncoderRead;
  str_msg2.data = EncoderRead2;

  chatter.publish( &str_msg );
  delay(5);
  chatter2.publish( &str_msg2 );
  delay(5);
  nh.spinOnce();
//  delay(1000);
}

long Encoder_Read(){
  
  long newLeft; // , newRight;
  newLeft = knobLeft.read();
  if (newLeft != positionLeft /*|| newRight != positionRight*/) {

    positionLeft = newLeft;
  }
 
  return newLeft;
  }

  long Encoder_Read2(){
  
  long newRight;
  newRight = knobRight.read();
  if (newRight != positionRight) {
    positionRight = newRight;
  }
 
  return newRight;
  }
