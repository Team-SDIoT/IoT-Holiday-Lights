function sendRGB(rgb){
    //add mqtt send logic
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

jQuery(document).ready(function($) {
	$('.colourOption').on('click', function() {
		
		$('#colourPicker *').removeClass('selectedColour');
		$(this).toggleClass('selectedColour');
        
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
