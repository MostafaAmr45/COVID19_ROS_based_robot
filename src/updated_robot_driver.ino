/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */
#include <Encoder.h>
#include <RobojaxBTS7960.h>

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>


//--------Right motor-----------
#define RPWM 5 // define pin 3 for RPWM pin (output)
#define R_EN 4 // define pin 2 for R_EN pin (input)
#define R_IS 13  // define pin 5 for R_IS pin (output)

#define LPWM 6 // define pin 6 for LPWM pin (output)
#define L_EN 7 // define pin 7 for L_EN pin (input)
#define L_IS 14 // define pin 8 for L_IS pin (output)
#define CW 1 //do not change
#define CCW 0 //do not change
#define debug 0//change to 0 to hide serial monitor debugging infornmation or set to 1 to view


//--------left motor-----------
#define RPWM2 8 // define pin 3 for RPWM pin (output)
#define R_EN2 10 // define pin 2 for R_EN pin (input)
#define R_IS2 14 // define pin 5 for R_IS pin (output)

#define LPWM2 9 // define pin 6 for LPWM pin (output)
#define L_EN2 11 // define pin 7 for L_EN pin (input)
#define L_IS2 12 // define pin 8 for L_IS pin (output)

RobojaxBTS7960 motor_right(R_EN,RPWM,R_IS, L_EN,LPWM,L_IS,debug);
RobojaxBTS7960 motor_left(R_EN2,RPWM2,R_IS2, L_EN2,LPWM2,L_IS2,debug);

Encoder knobRight(2, 3);
Encoder knobLeft(18, 19);


ros::NodeHandle  nh;
long EncoderRead_right=0;
long positionRight  = -99999;

long EncoderRead_Left=0;
long positionLeft  = -99999;


std_msgs::Int16 str_msg_left;
ros::Publisher chatter_left("lwheel_ticks", &str_msg_left);

std_msgs::Int16 str_msg_right;
ros::Publisher chatter_right("rwheel_ticks", &str_msg_right);


//publishing

//char hello[25] = "publishing on ros!";

//subscrbing
void messageCb_right (const std_msgs::Float32& Vmsg){
       int motor_right_value= int(Vmsg.data);
      
       if(motor_right_value>0 && motor_right_value<=255)
       {
         int x = map(motor_right_value, 0,255,0,100);
         motor_right.rotate(x,CW);
//         delay(50);
        }else if(motor_right_value<0 && motor_right_value>-255)
        { 
         int y = map(abs(motor_right_value),0, 255,0,100);
         motor_right.rotate(y,CCW);
//         delay(50);
          }else
          {
            motor_right.stop();
            }
}

void messageCb_left (const std_msgs::Float32& Vmsg){
       int motor_value_left= int(Vmsg.data);
      
       if(motor_value_left>0 && motor_value_left<=255)
       {
         int x2 = map(motor_value_left, 0,255,0,100);
         motor_left.rotate(x2,CW);
//         delay(50);
        }else if(motor_value_left<0 && motor_value_left>-255)
        { 
         int y2 = map(abs(motor_value_left),0, 255,0,100);
         motor_left.rotate(y2,CCW);
//         delay(50);
          }else
          {
            motor_left.stop();
            }

}

ros::Subscriber<std_msgs::Float32> sub_right("rmotor", messageCb_right);
ros::Subscriber<std_msgs::Float32> sub_left("lmotor", messageCb_left);


void setup()
{
  Serial.begin(57600);// setup Serial Monitor to display information

  nh.initNode();
    motor_right.begin();
    motor_left.begin();
//
  nh.advertise(chatter_right);
  nh.advertise(chatter_left);

  nh.subscribe(sub_right);  
  nh.subscribe(sub_left);
}

void loop()
{

  str_msg_left.data =Encoder_Read_left();;
  chatter_left.publish( &str_msg_left );

  str_msg_right.data = Encoder_Read_right();
  chatter_right.publish( &str_msg_right );
  nh.spinOnce();
//  delay(1000);
}

long Encoder_Read_right(){
  
  long newRight; 
  newRight = knobRight.read();
  if (newRight != positionRight ) {
    Serial.print("Right = ");
    Serial.print(newRight);
    Serial.println();
    positionRight = newRight;
  }


  return newRight;
  }

long Encoder_Read_left(){
  
  long newLeft; 
  newLeft = knobLeft.read();
  if (newLeft != positionLeft) {
    Serial.print("Left = ");
    Serial.print(newLeft);

    positionLeft = newLeft;
  }
 
  return newLeft;
  }
