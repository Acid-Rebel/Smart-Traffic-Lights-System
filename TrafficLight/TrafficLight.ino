struct Signal
{
  int Red;
  int Yellow;
  int Green;
  int Presence;
  int Sensor[2];
}*Signals[4];


enum Names{I=0,II=1,III=2,IV=3};
enum Color{Red=82,Yellow=89,Green=71,Presence=98,PresenceOff=99};

int prev=-1;
int *prevSignal=&prev;
//*prevSignal=-1;


int readUltrasonicDistance(int *triggerPin, int *echoPin)
{
  pinMode(*triggerPin, OUTPUT);  
  digitalWrite(*triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(*triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(*triggerPin, LOW);
  pinMode(*echoPin, INPUT);
  return (int)pulseIn(*echoPin, HIGH);
}

void highPin(int *Pin)
{
  digitalWrite(*Pin,HIGH);
}

void lowPin(int *Pin)
{
  digitalWrite(*Pin,LOW);
}

bool isCarPresent(int *Number)
{
  int distance=0;
    distance=0.01723*readUltrasonicDistance(&((*Signals[*Number]).Sensor[0]) ,&((*Signals[*Number]).Sensor[1]));

  if(distance>=1 && distance<=5)
      return true;
    return false;
}


void delaySignal(int Type,int *Number)
{
  switch(Type)
  {
    case Green:
      for(int k=1;k<=10;k++)
        delay(1);
        int flag=0;
        for(int i=(*(Number)==IV)?5:(*Number)+1;i<=IV;i++)
        {

           if(isCarPresent(&i))
          {
            setSignal(&i,Presence);
          }
          else
          {
            setSignal(&i,PresenceOff);
          }


          if(isCarPresent(Number))
          {
            setSignal(Number,Presence);
          }
          else
          {
            setSignal(Number,PresenceOff);
          }


          if(isCarPresent(&i)) 
          {
            setSignal(&i,Presence);
            if(!isCarPresent(Number))
            {
              setSignal(Number,PresenceOff);
              *prevSignal=*Number;
              setSignal(&i,Green);
              flag=1;
              break;
            }
          }
        }
        if(flag==1)
          break;
        for(int i=I;i<*Number;i++)
        {
          if(isCarPresent(&i))
          {
            setSignal(&i,Presence);
          }
          else
          {
            setSignal(&i,PresenceOff);
          }


          if(isCarPresent(Number))
          {
            setSignal(Number,Presence);
          }
          else
          {
            setSignal(Number,PresenceOff);
          }
          
          if(isCarPresent(&i)) 
          {
            if(!isCarPresent(Number))
            {
              *prevSignal=*Number;
              setSignal(&i,Green);
              flag=1;
              break;
            }
          }
        }
        if(flag==1)
          break;
        Serial.println(k);
      }
      break;
    case Yellow:
      delay(2000);
      break;
    case Red:   
      delay(5000);
      break;
  }
}

void setSignal(int *number,int ch)
{
  switch(ch)
  {
    case Red:
    	highPin(&((*Signals[*number]).Red));
  		lowPin(&((*Signals[*number]).Yellow));
  		lowPin(&((*Signals[*number]).Green));
    	break;

    case Yellow:
    	lowPin(&((*Signals[*number]).Red));
  		highPin(&((*Signals[*number]).Yellow));
  		lowPin(&((*Signals[*number]).Green));
      if(*prevSignal!=-1)
      {
        lowPin(&((*Signals[*prevSignal]).Red));
  		  highPin(&((*Signals[*prevSignal]).Yellow));
  		  lowPin(&((*Signals[*prevSignal]).Green));
      }
      delaySignal(Yellow, number);
    	break;

    case Green:
      setSignal(number,Yellow);
    	lowPin(&((*Signals[*number]).Red));
  		lowPin(&((*Signals[*number]).Yellow));
  		highPin(&((*Signals[*number]).Green));
      if(*prevSignal!=-1)
        setSignal(prevSignal,Red);
      delaySignal(Green,number);
      break;
    case Presence:
      highPin(&((*Signals[*number]).Presence));
      break;
    case PresenceOff:
      lowPin(&((*Signals[*number]).Presence));
      break;
  }
}

void initalize()
{
  for(int i=I;i<=IV;i++)
  {
    setSignal(&i,Red);
    //highPin((*Signals[i]).Presence);
  }
}


void setSignalBlue(int *number,int ch)
{
  switch(*number)
  {
    case 5:
      setSignalBlue(*prevSignal,Yellow);
      setSignalBlue(*prevSignal,Red);
      break;
    default:
      switch(ch)
      {
        case Red:
    	    highPin(&((*Signals[*number]).Red));
  		    lowPin(&((*Signals[*number]).Yellow));
  		    lowPin(&((*Signals[*number]).Green));
    	    break;

        case Yellow:
    	    lowPin(&((*Signals[*number]).Red));
  		    highPin(&((*Signals[*number]).Yellow));
  		    lowPin(&((*Signals[*number]).Green));
          if(*prevSignal!=-1)
          {
            lowPin(&((*Signals[*prevSignal]).Red));
  		      highPin(&((*Signals[*prevSignal]).Yellow));
  		      lowPin(&((*Signals[*prevSignal]).Green));
          }
          delay(3000);
    	    break;

        case Green:
          setSignal(number,Yellow);
    	    lowPin(&((*Signals[*number]).Red));
  		    lowPin(&((*Signals[*number]).Yellow));
  		    highPin(&((*Signals[*number]).Green));
          if(*prevSignal!=-1)
            setSignal(prevSignal,Red);
          break;
    }
  }
}




struct Signal a={13,12,11,22,{A0,A1}};
struct Signal b={10,9,8,23,{A2,A3}};
struct Signal c={7,6,5,24,{A4,A5}};
struct Signal d={4,3,2,25,{A6,A7}};


void setup()
{
    
    Signals[I]=&a;
    Signals[II]=&b;
    Signals[III]=&c;
    Signals[IV]=&d;
   
  	for(int i=I;i<=IV;i++)
    {
      pinMode((*Signals[i]).Red,OUTPUT);
      pinMode((*Signals[i]).Yellow,OUTPUT);
      pinMode((*Signals[i]).Green,OUTPUT);
      pinMode((*Signals[i]).Presence,OUTPUT);
    }
    //pinMode(18,OUTPUT);
    //pinMode(19,INPUT);
    //readUltrasonicDistance(&((*Signals[I]).Sensor[0]) , &((*Signals[I]).Sensor[1]));
    //readUltrasonicDistance(&((*Signals[I]).Sensor[0]) , &((*Signals[I]).Sensor[1]));
    Serial.begin(9600);
    //Serial.begin(115200);
    initalize();

}

void runNormal()
{
  for(int i=I;i<=IV;i++)
    {
      if(isCarPresent(&i))
      {
        setSignal(&i,Presence);
        if(*prevSignal!=i)
          setSignal(&i,Green);
        *prevSignal=i;
      }
      else
      {
        setSignal(&i,PresenceOff);
      }
  }
}

void runBlue()
{
  int ch=6;
  if(Serial.available())
  {
    ch=Serial.parseInt();
    Serial.println(ch);
    ch-=1;
    if(ch!=*prevSignal)
      setSignalBlue(&ch,Green);
    *prevSignal=ch;
    ch=6;
  }
}

void loop()
{
  runNormal();
  //runBlue();
}