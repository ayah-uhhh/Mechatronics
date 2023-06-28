void setup() {
  // put your setup code here, to run once:
pinMode(A0, INPUT);
pinMode(2, INPUT);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int dig = digitalRead(2);
  int analog = analogRead(A0); //center

  int dig2 = digitalRead(3);
  int analog2 = analogRead(A1); //left

  int dig3 = digitalRead(4);
  int analog3 = analogRead(A2); //right
  Serial.print(dig2);
  Serial.print(dig);
  Serial.println(dig3);


//  if(dig == 0) //center
//      {
//        digitalWrite(ENABLE,HIGH);
//        if(sped <= 255 && sped >= 0)
//        {
//        analogWrite(ENABLE,sped); // enable on
//        }
//        else
//        {
//        digitalWrite(ENABLE,LOW); // enable on
//        }
//      digitalWrite(DIRA,HIGH); //one way
//      digitalWrite(DIRB,LOW);
//    }
//  
//  if(dig == 1 && dig2 == 0) 
//        {//move left somehow
//          digitalWrite(ENABLE,HIGH);
//          if(sped <= 255 && sped >= 0)
//          {
//          analogWrite(ENABLE,sped); // enable on
//          }
//          else
//          {
//          digitalWrite(ENABLE,LOW); // enable on
//          }
//        digitalWrite(DIRA,HIGH); //one way
//        digitalWrite(DIRB,LOW);
//      }
//      
//  if(dig == 1 && dig3 == 0) 
//        {//move right somehow
//          digitalWrite(ENABLE,HIGH);
//          if(sped <= 255 && sped >= 0)
//          {
//          analogWrite(ENABLE,sped); // enable on
//          }
//          else
//          {
//          digitalWrite(ENABLE,LOW); // enable on
//          }
//        digitalWrite(DIRA,HIGH); //one way
//        digitalWrite(DIRB,LOW);
//      }
//      


}
