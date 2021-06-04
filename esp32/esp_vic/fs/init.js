/*
 * @author	Simon Ullerstam (ullr@cs.umu.se)
 * @created	2021-05-10
 * description:
 * 	ESP32 code for connecting remote 2 to MQTT.
 */

// API:s
load('api_uart.js');
load('api_mqtt.js');

// "Constants"
let uartNo = 1;
let topicRoot = 'pandebil/'; // MQTT topic root
// MQTT topics
let topic0 = 'to_car';
let topic1 = 'from_car';
let topic2 = 'to_rem1';
let topic3 = 'to_rem2';

// Configure UART (38400 baud, 8 data bits, 1 stop bit)
UART.setConfig(uartNo, {
	baudRate: 38400,
	numDataBits: 8,
	numStopBits: 1,
	rxBufSize: 256,
	rxLingerMicros: 0,
	esp32: {
		gpio: {
			rx: 16,
			tx: 17,
		},
		fifo: {
			rxFullThresh: 1,
			rxAlarm: 0,
		}
	},
});

// Set dispatcher callback whenver UART Rx recived,
UART.setDispatcher(uartNo, function(uartNo) {
	let ra = UART.readAvail(uartNo);
	if (ra > 0) {
		// Receive data
		let data = UART.read(uartNo);
		print('Received UART data:', data);

		// If connected
		if (MQTT.isConnected()) {
			// Parse up the different messages
			let msg = '';
			for (let i = 0; i < data.length; i++) {
				if (data[i] !== '\n') {
					msg += data[i];
				} else {
					// Check topic
					let topic = msg.slice(0,1);
					if (topic === '0') {
						topic = topic0;
					} else if (topic === '1') {
						topic = topic1;
					} else if (topic === '2') {
						topic = topic2;
					} else if (topic === '3') {
						topic = topic3;
					} else {
						topic = '';
					}

					// Existing topic
					if (topic !== '') {
						// Publish MQTT
						let res = MQTT.pub(topicRoot + topic, msg, 1);
						print('Pub:', res ? 'yes' : 'no', 'msg:', msg);	
					} else {
						print('Invalid topic');
					}
					msg = ''; // Reset for next loop
				}
			}
		} else {
			print('publish failed, MQTT not connected');
		}
	}
}, null);

// Enable Rx
UART.setRxEnabled(uartNo, true);

// Subscribe to topics (from_car & to_rem2), forward to UART Tx when data recived
MQTT.sub(topicRoot + topic1, function(conn, topic, msg) {
	// write UART
	UART.write(uartNo, msg + '\n');
	print('Topic:', topic, 'msg:', msg);
}, null);
MQTT.sub(topicRoot + topic3, function(conn, topic, msg) {
	// write UART
	UART.write(uartNo, msg + '\n');
	print('Topic:', topic, 'msg:', msg);
}, null);
