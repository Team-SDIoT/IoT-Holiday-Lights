// Create a client instance
var clientid = "testclient"+Math.floor(Math.random() * 20)+Math.floor(Math.random() * 20)+Math.floor(Math.random() * 20);
client = new Paho.MQTT.Client("13.126.106.71", Number("9001"),clientid);



// set callback handlers

client.onConnectionLost = onConnectionLost;

client.onMessageArrived = onMessageArrived;



// connect the client


client.connect(
{
				cleanSession : false, 
                onSuccess : onConnect, 
                onFailure : onConnectionLost, 
                keepAliveInterval: 30, 
				}

);




// called when the client connects

function onConnect() {

  // Once a connection has been made, make a subscription and send a message.

  console.log("onConnect");

  //client.subscribe("World");

  message = new Paho.MQTT.Message("Hello");

  message.destinationName = "test";

  client.send(message);

}

function sendRGB(rgb){
	rgb = "" + rgb;
	 var message = new Paho.MQTT.Message(rgb);
	 message.destinationName = "cafeteria/trees";
	 client.send(message);
	 console.log(message);
    console.log(rgb);
}


// called when the client loses its connection

function onConnectionLost(responseObject) {


  if (responseObject.errorCode !== 0) {

    console.log("onConnectionLost:"+responseObject.errorMessage);

  }

this.connect(
{
				cleanSession : false, 
                onSuccess : onConnect, 
                onFailure : onConnectionLost, 
                keepAliveInterval: 30, 
				}

);

}
function sendColorWipe() {
 //var mess = "color-wipe";
 sendRGB("color-wipe");
}

function sendRainbow() {
 sendRGB("rainbow");
}

function sendRainbowCycle() {
 sendRGB("rainbow-cycle");
}

function sendTheaterChase() {
 sendRGB("theater-chase");
}



// called when a message arrives

function onMessageArrived(message) {

  console.log("onMessageArrived:"+message.payloadString);

}

jQuery(document).ready(function($) {

	/* Some simple jQuery to switch the classes */
	$('.colourOption').on('click', function() {
		/* This line removes the selectedColour class from every element in the colouPicker, meaning that only one is selected at a time!

		 It basically saves me having to make them a check box, I should proabbly be told of for that...*/
		$('#colourPicker *').removeClass('selectedColour');
		/* This switches on the selectedColour class for that div. */
		$(this).toggleClass('selectedColour');

    /*Changing the body colours, its really ugly repeative code, I could probably improve it! */
		if ($(this).attr('id') == 'colourOne') {
			$('polygon').addClass('bodyOne');
			$('polygon').removeClass('bodyTwo bodyThree bodyFour bodyFive bodySix');
		} else if ($(this).attr('id') == 'colourTwo') {
			$('polygon').addClass('bodyTwo');
			$('polygon').removeClass('bodyOne bodyThree bodyFour bodyFive bodySix');
		} else if ($(this).attr('id') == 'colourThree') {
			$('polygon').addClass('bodyThree');
			$('polygon').removeClass('bodyOne bodyTwo bodyFour bodyFive bodySix');
		} else if ($(this).attr('id') == 'colourFour') {
			$('polygon').addClass('bodyFour');
			$('polygon').removeClass('bodyOne bodyTwo bodyThree bodyFive bodySix');
		} else if ($(this).attr('id') == 'colourFive') {
			$('polygon').addClass('bodyFive');
			$('polygon').removeClass('bodyOne bodyTwo bodyThree bodyFour bodySix');
		} else if ($(this).attr('id') == 'colourSix') {
			$('polygon').addClass('bodySix');
			$('polygon').removeClass('bodyOne bodyTwo bodyThree bodyFour bodyFive');
		}

		var rgb = $('polygon').css('fill');

		rgb = rgb.replace(/[^\d,]/g, '').split(',');
		sendRGB(rgb);
	});

});
