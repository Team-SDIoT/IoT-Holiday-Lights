import logging
import os
import paho.mqtt.client as mqtt

from flask import Flask
from flask_ask import Ask, request, session, question, statement
from time import sleep

app = Flask(__name__)
ask = Ask(app, "/")
logging.getLogger('flask_ask').setLevel(logging.DEBUG)

mqttc = mqtt.Client()
mqttc.connect("13.126.106.71", 1883, 60)
mqttc.loop_start()


@ask.launch
def launch():
    speech_text = 'Merrry Christmas...! . Welcome to the IoT christmas holiday lights by S.D.I.O.T.!. Tell me a color'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)
    #return statement(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('OnIntent')
def on():
    value = '50' + ',' + '50' + ',' + '50'
    mqttc.publish("cafeteria/trees", value)
    sleep(1)
    value = '100' + ',' + '100' + ',' + '100'
    mqttc.publish("cafeteria/trees", value)
    sleep(1)
    value = '255' + ',' + '255' + ',' + '255'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned on!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

    #return statement(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('OffIntent')
def off():
    value = '0' + ',' + '0' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned off!'
    return statement(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('BlueIntent')
def blue():
    value = '0' + ',' + '0' + ',' + '255'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Blue! New color?'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('PurpleIntent')
def purple():
    value = '128' + ',' + '0' + ',' + '128'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Purple! New color?'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('GreenIntent')
def green():
    value = '0' + ',' + '255' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Green New color?!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('RedIntent')
def red():
    value = '255' + ',' + '0' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Red! New color?'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('OrangeIntent')
def orange():
    value = '255' + ',' + '165' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Orange! New color?'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('YellowIntent')
def yellow():
    value = '255' + ',' + '255' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Yellow! New color?'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('ColorWipeIntent')
def wipe():
    value = '255' + ',' + '255' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Wipe Animation activated'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('TheaterChaseIntent')
def theater():
    value = 'theater-chase'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Chase Animation activated'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)


@ask.intent('RainbowIntent')
def rainbow():
    mqttc.publish("cafeteria/trees", "rainbow")
    speech_text = 'Rainbow Animation activated'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)        

@ask.intent('AMAZON.HelpIntent')
def help():
    speech_text = 'You can start SDIoT christmas tree lights!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)   


@ask.session_ended
def session_ended():
    return "{}", 200


if __name__ == '__main__':
    if 'ASK_VERIFY_REQUESTS' in os.environ:
        verify = str(os.environ.get('ASK_VERIFY_REQUESTS', '')).lower()
        if verify == 'false':
            app.config['ASK_VERIFY_REQUESTS'] = False
app.run(debug=True)
