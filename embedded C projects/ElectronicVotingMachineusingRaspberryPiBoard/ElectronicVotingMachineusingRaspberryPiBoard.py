import http.client
   import urllib
   import RPi.GPIO as GPIO
   import time

   sleep = 60
   key = 'QHHBG1DGHX64M6MM'  # Thingspeak channel to update

   GPIO.setmode(GPIO.BCM)
   GPIO.setwarnings(False)
   GPIO.setup(23,GPIO.IN,pull_up_down=GPIO.PUD_UP)
   GPIO.setup(24,GPIO.OUT)
   GPIO.setup(14,GPIO.IN,pull_up_down=GPIO.PUD_UP)
   GPIO.setup(15,GPIO.OUT)
   GPIO.setup(18,GPIO.IN,pull_up_down=GPIO.PUD_UP)
   GPIO.setup(2,GPIO.OUT)
   GPIO.setup(3,GPIO.IN,pull_up_down=GPIO.PUD_UP)
   GPIO.setup(4,GPIO.OUT)
   GPIO.setup(17,GPIO.IN,pull_up_down=GPIO.PUD_UP)
   BJP=0
   CNG=0
   TDP=0
   YSRCP=0
try:
    while True:
        button_state1=GPIO.input(23)
        button_state2=GPIO.input(14)
        button_state3=GPIO.input(18)
        button_state4=GPIO.input(3)
        button_state5=GPIO.input(17)
        if button_state1==False:
            BJP=BJP+1
            GPIO.output(24,True)
            print('your vote was registered for BJP')
            time.sleep(0.2)
            def thermometer():
                while True:
                    #Calculate CPU temperature of Raspberry Pi in Degrees C
                    temp = 1
                    params = urllib.parse.urlencode({'field2': temp, 'key':key }) 
                    headers = {"Content-typZZe": "application/x-www-form-urlencoded","Accept": "text/plain"}
                    conn = http.client.HTTPConnection("api.thingspeak.com:80")
                    try:
                        conn.request("POST", "/update", params, headers)
                        response = conn.getresponse()
                        print ("x")
                    except:
                        print ("connection failed")
                    break
            if __name__ == "__main__":
                    while True:
                        thermometer()
                        time.sleep(sleep)
                        break
                        
        elif button_state2==False:
            CNG=CNG+1
            GPIO.output(15,True)
            print('your vote was registered for CNG')
            time.sleep(0.2)
        elif button_state3==False:
            TDP=TDP+1
            GPIO.output(2,True)
            print('your vote was registered for TDP')
            time.sleep(0.2)
        elif button_state4==False:
            YSRCP=YSRCP+1
            GPIO.output(4,True)
            print('your vote was registered for YSRCP')
            time.sleep(0.2)
        elif button_state5==False:
            if (BJP>CNG):
                if(BJP>TDP ):
                    if(BJP>YSRCP):
                        print('BJP WON',BJP)
                        time.sleep(0.2)
                    else:
                        print('ysrcp won',YSRCP)
                        time.sleep(0.2)
                else:
                    if(TDP>YSRCP):
                        print('tdp won',TDP)
                        time.sleep(0.2)
                    else:
                        print('ysrcp won',YSRCP)
                        time.sleep(0.2)
            else:
                if(CNG>TDP):
                    if(CNG>YSRCP):
                        print('cng won',CNG)
                        time.sleep(0.2)
                    else:
                        print('ysrcp won',YSRCP)
                        time.sleep(0.2)
                else:
                    if(TDP>YSRCP):
                        print('tdp won',TDP)
                        time.sleep(0.2)
                    else:
                        print('ysrcp won',YSRCP)
                        time.sleep(0.2)
                        
        
        else:
            GPIO.output(24,False)
            GPIO.output(15,False)
            GPIO.output(2,False)
            GPIO.output(4,False)
except:
    GPIO.cleanup()
