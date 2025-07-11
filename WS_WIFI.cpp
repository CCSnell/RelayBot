#include "WS_WIFI.h"

// The name and password of the WiFi access point
const char *ssid = APSSID;                
const char *password = APPSK;               
IPAddress apIP(192, 168, 4, 1);    // Set the IP address of the AP

char ipStr[16];
WebServer server(80);
Preferences preferences;  // For persistent storage

void handleRoot() {
  String myhtmlPage =
    String("") +
    "<html>"+
    "<head>"+
    "    <meta charset=\"utf-8\">"+
    "    <title>ESP32-S3-POE-ETH-8DI-8RO</title>"+
    "    <style>" +
    "        body {" +
    "            font-family: Arial, sans-serif;" +
    "            background-color: #f0f0f0;" +
    "            margin: 0;" +
    "            padding: 0;" +
    "        }" +
    "        .header {" +
    "            text-align: center;" +
    "            padding: 20px 0;" +
    "            background-color: #333;" +
    "            color: #fff;" +
    "            margin-bottom: 20px;" +
    "        }" +
    "        .container {" +
    "            max-width: 600px;" +
    "            margin: 0 auto;" +
    "            padding: 20px;" +
    "            background-color: #fff;" +
    "            border-radius: 5px;" +
    "            box-shadow: 0 0 5px rgba(0, 0, 0, 0.3);" +
    "        }" +
    "        .input-container {" +
    "            display: flex;" +
    "            align-items: center;" +
    "            margin-bottom: 10px;" +
    "        }" +
    "        .input-container label {" +
    "            width: 50px;" + 
    "            margin-right: 10px;" +
    "        }" +
    "        .input-container input[type=\"text\"] {" +
    "            width: 80px;" +
    "            padding: 5px;" +
    "            border: 1px solid #ccc;" +
    "            border-radius: 3px;" +
    "            margin-right: 10px; "+ 
    "        }" +
    "        .input-container input[type=\"number\"] {" +
    "            width: 80px;" +
    "            padding: 5px;" +
    "            border: 1px solid #ccc;" +
    "            border-radius: 3px;" +
    "            margin-right: 10px; "+ 
    "        }" +
    "        .input-container button {" +
    "            padding: 5px 10px;" +
    "            background-color: #333;" +
    "            color: #fff;" +
    "            font-size: 12px;" +
    "            font-weight: bold;" +
    "            border: none;" +
    "            border-radius: 3px;" +
    "            text-transform: uppercase;" +
    "            cursor: pointer;" +
    "            margin-right: 5px;" +
    "        }" +
    "        .button-container {" +
    "            margin-top: 20px;" +
    "            text-align: center;" +
    "        }" +
    "        .button-container button {" +
    "            margin: 0 5px;" +
    "            padding: 10px 15px;" +
    "            background-color: #333;" +
    "            color: #fff;" +
    "            font-size: 14px;" +
    "            font-weight: bold;" +
    "            border: none;" +
    "            border-radius: 3px;" +
    "            text-transform: uppercase;" +
    "            cursor: pointer;" +
    "        }" +
    "        .button-container button:hover {" +
    "            background-color: #555;" +
    "        }" +

    "        .channel-row {" +
    "            display: flex;" +
    "            align-items: center;" +
    "            margin-bottom: 8px;" +
    "        }" +
    "        .channel-row label {" +
    "            width: 50px;" +
    "            margin-right: 10px;" +
    "        }" +
    "        .channel-row input {" +
    "            width: 80px;" +
    "            padding: 4px;" +
    "            border: 1px solid #ccc;" +
    "            border-radius: 3px;" +
    "            margin-right: 10px;" +
    "        }" +
    "        .channel-row button {" +
    "            padding: 4px 8px;" +
    "            background-color: #28a745;" +
    "            color: white;" +
    "            border: none;" +
    "            border-radius: 3px;" +
    "            cursor: pointer;" +
    "            font-size: 12px;" +
    "        }" +
    "        .channel-row button:hover {" +
    "            background-color: #218838;" +
    "        }" +
    "        .settings-header {" +
    "            font-weight: bold;" +
    "            margin-bottom: 10px;" +
    "            font-size: 12px;" +
    "            color: #666;" +
    "        }" +
    "        nav {" +
    "            margin: 15px 0;" +
    "            text-align: center;" +
    "        }" +
    "        nav a {" +
    "            padding: 10px 50px;" +
    "            background-color: #333;" +
    "            color: white;" +
    "            text-decoration: none;" +
    "            font-weight: bold;" +
    "            border-radius: 5px;" +
    "        }" +
    "        nav a.relayControlActive {" + 
    "            background-color: #fff;" +   
    "            color: #333;" +
    "            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3), 0 1px 3px rgba(0, 0, 0, 0.1);" +
    "            transform: translateY(-4px);" +
    "            transition: all 0.2s ease-in-out;" +
    "        }" + 
    "        nav a.rtcEventActive {" + 
    "            background-color: #555;" +
    "        }" + 

    "    </style>" +
    "</head>"+
    "<body>"+
    "    <script defer=\"defer\">"+
    "        function ledSwitch(ledNumber) {"+
    "            var xhttp = new XMLHttpRequest();" +
    "            xhttp.onreadystatechange = function() {" +
    "                if (this.readyState == 4 && this.status == 200) {" +
    "                    console.log('LED ' + ledNumber + ' state changed');" +
    "                }" +
    "            };" +
    "            if (ledNumber < 9 && ledNumber > 0) {" +
    "             xhttp.open('GET', '/Switch' + ledNumber, true);" +
    "            }" +
    "            else if(ledNumber == 9){" +
    "            xhttp.open('GET', '/AllOn', true);" +
    "            }" +
    "            else if(ledNumber == 0){" +
    "            xhttp.open('GET', '/AllOff', true);" +
    "            }" +
    "            xhttp.send();" +
    "        }" +
    "        function pulseRelay(channel) {" +
    "            var timeInput = document.getElementById('time' + channel);" +
    "            var pulseTime = timeInput.value ? parseInt(timeInput.value) : null;" +
    "            var url = '/PulseRelay?channel=' + channel;" +
    "            if (pulseTime !== null && !isNaN(pulseTime)) {" +
    "                if (pulseTime < 10 || pulseTime > 10000) {" +
    "                    alert('Please enter a valid time between 10 and 10000 milliseconds');" +
    "                    return;" +
    "                }" +
    "                url += '&time=' + pulseTime;" +
    "            }" +
    "            var xhttp = new XMLHttpRequest();" +
    "            xhttp.onreadystatechange = function() {" +
    "                if (this.readyState == 4 && this.status == 200) {" +
    "                    console.log('Pulse started for channel ' + channel);" +
    "                }" +
    "            };" +
    "            xhttp.open('GET', url, true);" +
    "            xhttp.send();" +
    "        }" +

    "        function saveChannelSettings(channel) {" +
    "            var timeInput = document.getElementById('time' + channel);" +
    "            var countInput = document.getElementById('count' + channel);" +
    "            var time = parseInt(timeInput.value);" +
    "            var count = parseInt(countInput.value);" +
    "            " +
    "            var hasValidTime = !isNaN(time) && time >= 10 && time <= 10000;" +
    "            var hasValidCount = !isNaN(count) && count >= 1 && count <= 10;" +
    "            " +
    "            if (!hasValidTime && !hasValidCount) {" +
    "                alert('Please enter valid time (10-10000ms) and/or count (1-10 cycles)');" +
    "                return;" +
    "            }" +
    "            " +
    "            var url = '/SetChannelPulseSettings?channel=' + channel;" +
    "            if (hasValidTime) url += '&time=' + time;" +
    "            if (hasValidCount) url += '&count=' + count;" +
    "            " +
    "            var xhttp = new XMLHttpRequest();" +
    "            xhttp.onreadystatechange = function() {" +
    "                if (this.readyState == 4 && this.status == 200) {" +
    "                    console.log('Channel settings saved for channel ' + channel);" +
    "                }" +
    "            };" +
    "            xhttp.open('GET', url, true);" +
    "            xhttp.send();" +
    "        }" +
    "        function loadChannelSettings() {" +
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/GetChannelPulseSettings', true);" +
    "            xhr.onreadystatechange = function() {" +
    "                if (xhr.readyState === 4 && xhr.status === 200) {" +
    "                    var settings = JSON.parse(xhr.responseText);" +
    "                    for (var i = 0; i < settings.durations.length; i++) {" +
    "                        var timeInput = document.getElementById('time' + (i + 1));" +
    "                        var countInput = document.getElementById('count' + (i + 1));" +
    "                        if (timeInput) timeInput.value = settings.durations[i];" +
    "                        if (countInput) countInput.value = settings.counts[i];" +
    "                    }" +
    "                }" +
    "            };" +
    "            xhr.send();" +
    "        }" +
    "        function updateData() {" +
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/getData', true);" +
    "            xhr.onreadystatechange = function() {" +
    "              if (xhr.readyState === 4 && xhr.status === 200) {" +
    "                var dataArray = JSON.parse(xhr.responseText);" +
    "                document.getElementById('ch1').value = dataArray[0];" +
    "                document.getElementById('ch2').value = dataArray[1];" +
    "                document.getElementById('ch3').value = dataArray[2];" +
    "                document.getElementById('ch4').value = dataArray[3];" +
    "                document.getElementById('ch5').value = dataArray[4];" +
    "                document.getElementById('ch6').value = dataArray[5];" +
    "                document.getElementById('ch7').value = dataArray[6];" +
    "                document.getElementById('ch8').value = dataArray[7];" +
    // "                // Remove the button's disabled attribute to make it clickable"+
    "                updateRelayButtons();" +
    "              }" +
    "            };" +
    "            xhr.send();" +
    "        }" +

    "        function updateRelayButtons() {" +
    "            // Always enable all relay control buttons" +
    "            for (var i = 1; i <= 8; i++) {" +
    "                document.getElementById('btn' + i).disabled = false;" +
    "            }" +
    "            document.getElementById('btn9').disabled = false;" +  // All On
    "            document.getElementById('btn0').disabled = false;" +  // All Off
    "        }" +
    "        function displayErrorTextBox(show) {" +
    "          var errorTextbox = document.getElementById('errorTextbox');" +
    "          errorTextbox.style.display = show ? 'block' : 'none';" +
    "        }" +
    "        function resetErrorTextBox() {" +
    "          document.getElementById(\'errorTextbox\').value = \'\';" +
    "        }" +
    "        var refreshInterval = 200;" +                                     
    "        setInterval(updateData, refreshInterval);" +
    "        window.onload = function() { loadChannelSettings(); updateRelayButtons(); };" +
    "    </script>" +
    "    <div class=\"header\">"+
    "        <h1>ESP32-S3-POE-ETH-8DI-8RO</h1>"+
    "    </div>"+
    "    <nav>" +
    "        <a href=\"/\" id=\"relayControlLink\" class=\"relayControlActive\">Relay Control</a>" +  
    "        <a href=\"/RTC_Event\" id=\"rtcEventLink\" class=\"rtcEventActive\">RTC Event</a>" + 
    "    </nav>" +

    "    <div class=\"container\">"+

    "        <div class=\"input-container\" style=\"margin-left: 20px;\">"+
    "            <label for=\"input1\">CH1</label>"+
    "            <input type=\"text\" id=\"ch1\" />"+
    "            <button value=\"Switch1\" id=\"btn1\" onclick=\"ledSwitch(1)\">Toggle</button>"+
    "            <input type=\"number\" id=\"time1\" placeholder=\"ms\" min=\"10\" max=\"10000\" value=\"500\" />"+
    "            <button onclick=\"pulseRelay(1)\">Pulse</button>"+
    "            <input type=\"number\" id=\"count1\" placeholder=\"cycles\" min=\"1\" max=\"10\" value=\"2\" />"+
    "            <button onclick=\"saveChannelSettings(1)\">SAVE</button>"+
    "        </div>"+
    "        <div class=\"input-container\" style=\"margin-left: 20px;\">"+
    "            <label for=\"input2\">CH2</label>"+
    "            <input type=\"text\" id=\"ch2\" />"+
    "            <button value=\"Switch2\" id=\"btn2\" onclick=\"ledSwitch(2)\">Toggle</button>"+
    "            <input type=\"number\" id=\"time2\" placeholder=\"ms\" min=\"10\" max=\"10000\" value=\"500\" />"+
    "            <button onclick=\"pulseRelay(2)\">Pulse</button>"+
    "            <input type=\"number\" id=\"count2\" placeholder=\"cycles\" min=\"1\" max=\"10\" value=\"2\" />"+
    "            <button onclick=\"saveChannelSettings(2)\">SAVE</button>"+
    "        </div>"+
    "        <div class=\"input-container\" style=\"margin-left: 20px;\">"+
    "            <label for=\"input3\">CH3</label>"+
    "            <input type=\"text\" id=\"ch3\" />"+
    "            <button value=\"Switch3\" id=\"btn3\" onclick=\"ledSwitch(3)\">Toggle</button>"+
    "            <input type=\"number\" id=\"time3\" placeholder=\"ms\" min=\"10\" max=\"10000\" value=\"500\" />"+
    "            <button onclick=\"pulseRelay(3)\">Pulse</button>"+
    "            <input type=\"number\" id=\"count3\" placeholder=\"cycles\" min=\"1\" max=\"10\" value=\"2\" />"+
    "            <button onclick=\"saveChannelSettings(3)\">SAVE</button>"+
    "        </div>"+
    "        <div class=\"input-container\" style=\"margin-left: 20px;\">"+
    "            <label for=\"input4\">CH4</label>"+
    "            <input type=\"text\" id=\"ch4\" />"+
    "            <button value=\"Switch4\" id=\"btn4\" onclick=\"ledSwitch(4)\">Toggle</button>"+
    "            <input type=\"number\" id=\"time4\" placeholder=\"ms\" min=\"10\" max=\"10000\" value=\"500\" />"+
    "            <button onclick=\"pulseRelay(4)\">Pulse</button>"+
    "            <input type=\"number\" id=\"count4\" placeholder=\"cycles\" min=\"1\" max=\"10\" value=\"2\" />"+
    "            <button onclick=\"saveChannelSettings(4)\">SAVE</button>"+
    "        </div>"+
    "        <div class=\"input-container\" style=\"margin-left: 20px;\">"+
    "            <label for=\"input5\">CH5</label>"+
    "            <input type=\"text\" id=\"ch5\" />"+
    "            <button value=\"Switch5\" id=\"btn5\" onclick=\"ledSwitch(5)\">Toggle</button>"+
    "            <input type=\"number\" id=\"time5\" placeholder=\"ms\" min=\"10\" max=\"10000\" value=\"500\" />"+
    "            <button onclick=\"pulseRelay(5)\">Pulse</button>"+
    "            <input type=\"number\" id=\"count5\" placeholder=\"cycles\" min=\"1\" max=\"10\" value=\"2\" />"+
    "            <button onclick=\"saveChannelSettings(5)\">SAVE</button>"+
    "        </div>"+
    "        <div class=\"input-container\" style=\"margin-left: 20px;\">"+
    "            <label for=\"input6\">CH6</label>"+
    "            <input type=\"text\" id=\"ch6\" />"+
    "            <button value=\"Switch6\" id=\"btn6\" onclick=\"ledSwitch(6)\">Toggle</button>"+
    "            <input type=\"number\" id=\"time6\" placeholder=\"ms\" min=\"10\" max=\"10000\" value=\"500\" />"+
    "            <button onclick=\"pulseRelay(6)\">Pulse</button>"+
    "            <input type=\"number\" id=\"count6\" placeholder=\"cycles\" min=\"1\" max=\"10\" value=\"2\" />"+
    "            <button onclick=\"saveChannelSettings(6)\">SAVE</button>"+
    "        </div>"+
    "        <div class=\"input-container\" style=\"margin-left: 20px;\">"+
    "            <label for=\"input7\">CH7</label>"+
    "            <input type=\"text\" id=\"ch7\" />"+
    "            <button value=\"Switch7\" id=\"btn7\" onclick=\"ledSwitch(7)\">Toggle</button>"+
    "            <input type=\"number\" id=\"time7\" placeholder=\"ms\" min=\"10\" max=\"10000\" value=\"500\" />"+
    "            <button onclick=\"pulseRelay(7)\">Pulse</button>"+
    "            <input type=\"number\" id=\"count7\" placeholder=\"cycles\" min=\"1\" max=\"10\" value=\"2\" />"+
    "            <button onclick=\"saveChannelSettings(7)\">SAVE</button>"+
    "        </div>"+
    "        <div class=\"input-container\" style=\"margin-left: 20px;\">"+
    "            <label for=\"input8\">CH8</label>"+
    "            <input type=\"text\" id=\"ch8\" />"+
    "            <button value=\"Switch8\" id=\"btn8\" onclick=\"ledSwitch(8)\">Toggle</button>"+
    "            <input type=\"number\" id=\"time8\" placeholder=\"ms\" min=\"10\" max=\"10000\" value=\"500\" />"+
    "            <button onclick=\"pulseRelay(8)\">Pulse</button>"+
    "            <input type=\"number\" id=\"count8\" placeholder=\"cycles\" min=\"1\" max=\"10\" value=\"2\" />"+
    "            <button onclick=\"saveChannelSettings(8)\">SAVE</button>"+
    "        </div>"+
    "        <div class=\"button-container\">"+
    "            <button value=\"AllOn\" id=\"btn9\" onclick=\"ledSwitch(9)\">All On</button>"+
    "            <button value=\"AllOff\" id=\"btn0\" onclick=\"ledSwitch(0)\">All Off</button>"+
    "        </div>"+
    "        <div id=\"errorTextbox\" style=\"display: none;\"> "+
    "            <p>English:Please refresh the page</p>"+      
    "            <p>Chinese:请刷新页面</p>"+
    "        </div>"+
    "    </div>"+
    "</body>"+
    "</html>";
    
  server.send(200, "text/html", myhtmlPage); 
  printf("The user visited the home page\r\n");
  
}
void handleRTCPage() {      
    String rtcPage = String("") + 
    "<html>" + 
    "<head>" + 
    "    <meta charset=\"utf-8\">" + 
    "    <title>ESP32-S3-POE-ETH-8DI-8RO</title>" + 
    "    <style>" + 
    "        body {" + 
    "            font-family: Arial, sans-serif;" + 
    "            background-color: #f0f0f0;" + 
    "            margin: 0;" + 
    "            padding: 0;" + 
    "        }" + 
    "        .header {" + 
    "            text-align: center;" + 
    "            padding: 20px 0;" + 
    "            background-color: #333;" + 
    "            color: #fff;" + 
    "            margin-bottom: 20px;" + 
    "        }" + 
    "        .container {" + 
    "            max-width: 600px;" + 
    "            margin: 10px auto;" + 
    "            padding: 20px;" + 
    "            background-color: #fff;" + 
    "            border-radius: 5px;" + 
    "            box-shadow: 0 0 5px rgba(0, 0, 0, 0.3);" + 
    "        }" + 
    "        .form-group {" + 
    "            margin-bottom: 15px;" + 
    "        }" + 
    "        .form-group label {" + 
    "            display: block;" + 
    "            font-weight: bold;" + 
    "        }" + 
    "        .form-group input {" + 
    "            width: 80px;" + 
    "            height: 25px;" + 
    "            padding: 4px;" + 
    "            margin-top: 5px;" + 
    "            border: 1px solid #ddd;" + 
    "            border-radius: 4px;" + 
    "            box-sizing: border-box;" + 
    "            text-align: right; " + 
    "        }" + 
    "        .form-group select {" + 
    "            width: 80px;" + 
    "            height: 25px;" + 
    "            padding: 4px;" + 
    "            margin-top: 5px;" + 
    "            border: 1px solid #ddd;" + 
    "            border-radius: 4px;" + 
    "            box-sizing: border-box;" + 
    "            text-align: right; " + 
    "        }" + 
    "        .form-group .btn {" + 
    "            padding: 10px 20px;" + 
    "            background-color: #333;" + 
    "            color: white;" + 
    "            border: none;" + 
    "            border-radius: 5px;" + 
    "            cursor: pointer;" + 
    "        }" + 
    "        .form-group .btn:hover {" + 
    "            background-color: #555;" + 
    "        }" + 
    "        .Events{"+
    "            font-size: 13px;"+
    "            word-wrap: break-word;"+
    "            overflow-wrap: break-word;"+
    "            max-width: 100%;"+
    "            white-space: nowrap;"+
    "            padding: 2px;"+
    "        }"+
    "        .Events button {"+
    "            float: right;" + 
    "            margin-left: 1px;" + 
    "        }"+
    "        .Events li {"+
    "            font-size: 13px;" + 
    "        }"+
    "        nav {" + 
    "            margin: 15px 0;" + 
    "            text-align: center;" + 
    "        }" + 
    "        nav a {" + 
    "            padding: 10px 50px;" + 
    "            background-color: #333;" + 
    "            color: white;" + 
    "            text-decoration: none;" + 
    "            font-weight: bold;" + 
    "            border-radius: 5px;" + 
    "        }" + 
    "        nav a.relayControlActive {" + 
    "            background-color: #555;" + 
    "        }" + 
    "        nav a.rtcEventActive {" + 
    "            background-color: #fff;" + 
    "            color: #333;" + 
    "            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3), 0 1px 3px rgba(0, 0, 0, 0.1);" + 
    "            transform: translateY(-4px);" + 
    "            transition: all 0.2s ease-in-out;" + 
    "        }" + 
    "    </style>" + 
    "</head>" + 
    "<body>" +
    "    <script defer=\"defer\">" +
    "        function getRtcEventData() {" +
    "            var dateBox1 = document.getElementById('DateBox1').value;" +
    "            var dateBox2 = document.getElementById('DateBox2').value;" +
    "            var dateBox3 = document.getElementById('DateBox3').value;" +
    "            var week = document.getElementById('Week').value;" +
    "            var timeBox1 = document.getElementById('TimeBox1').value;" +
    "            var timeBox2 = document.getElementById('TimeBox2').value;" +
    "            var timeBox3 = document.getElementById('TimeBox3').value;" +
    "            var relayCH1 = document.getElementById('RelayCH1').value;" +
    "            var relayCH2 = document.getElementById('RelayCH2').value;" +
    "            var relayCH3 = document.getElementById('RelayCH3').value;" +
    "            var relayCH4 = document.getElementById('RelayCH4').value;" +
    "            var relayCH5 = document.getElementById('RelayCH5').value;" +
    "            var relayCH6 = document.getElementById('RelayCH6').value;" +
    "            var relayCH7 = document.getElementById('RelayCH7').value;" +
    "            var relayCH8 = document.getElementById('RelayCH8').value;" +
    "            var cycleBox = document.getElementById('CycleBox').value;" +
    "            var WebData = " +
    "                'Date: ' + dateBox1 + '/' + dateBox2 + '/' + dateBox3 + '  ' + '\\n' + " + 
    "                'Week: ' + week + '  ' + '\\n' + " + 
    "                'Time: ' + timeBox1 + ':' + timeBox2 + ':' + timeBox3 +  '  ' + '\\n' + " + 
    "                'Relay CH1: ' + relayCH1 + '  ' + '\\n' + " + 
    "                'Relay CH2: ' + relayCH2 + '  ' + '\\n' + " + 
    "                'Relay CH3: ' + relayCH3 + '  ' + '\\n' + " + 
    "                'Relay CH4: ' + relayCH4 + '  ' + '\\n' + " + 
    "                'Relay CH5: ' + relayCH5 + '  ' + '\\n' + " + 
    "                'Relay CH6: ' + relayCH6 + '  ' + '\\n' + " + 
    "                'Relay CH7: ' + relayCH7 + '  ' + '\\n' + " + 
    "                'Relay CH8: ' + relayCH8 + '  ' + '\\n' + " + 
    "                'Cycle: ' + cycleBox + '  ' + '\\n' ;" + 
    "            var xhr = new XMLHttpRequest();" +
    "            xhr.open('GET', '/NewEvent?data=' + WebData, true);" +
    "            xhr.send();" +
    "        }" +
    "        function deleteEvent(eventId) {"+
    "            var xhr = new XMLHttpRequest();"+
    "            var EventId = eventId;"+
    "            xhr.open('GET', '/DeleteEvent?id=' + EventId, true);"+
    "            xhr.send();"+
    "        }"+
    "        function updateList(data) {"+
    "            var list = document.getElementById(\"myList\");"+
    "            list.innerHTML = \'\'; "+
    "            for (let i = 0; i < data.eventCount; i++) {"+
    "                var newItem = document.createElement(\"li\");"+
    "                var eventContent = data[\"eventStr\" + (i + 1)].replace(/\\n/g, \"<br>\");"+
    "                newItem.innerHTML = eventContent;"+
    "                var eventButton = document.createElement(\"button\");"+
    "                eventButton.textContent = \"Delete\" + \"Event\" + (i + 1);"+
    "                eventButton.onclick = function() {"+
    "                    deleteEvent(i+1);"+
    "                };"+
    "                newItem.style.display = 'flex';"+
    "                newItem.style.justifyContent = 'space-between';"+
    "                newItem.style.alignItems = 'center';"+
    "                newItem.appendChild(eventButton);"+
    "                list.appendChild(newItem);"+
    "            }"+
    "        }"+
    "        function upTime() {"+
    "            var xhr = new XMLHttpRequest();"+
    "            xhr.open('GET', '/getTimeAndEvent', true); "+
    "            xhr.onreadystatechange = function() {"+
    "                if (xhr.readyState === 4 && xhr.status === 200) {"+
    "                    var data = JSON.parse(xhr.responseText); "+
    "                    document.getElementById(\"Time\").textContent = data.time;"+
    "                    updateList(data); "+
    "                }"+
    "            };"+
    "            xhr.send();"+
    // "        fetch(\'/getTimeAndEvent\')"+
    // "            .then(response => response.json())"+
    // "            .then(data => {"+
    // "                document.getElementById(\"Time\").textContent = data.time;"+
    // "                updateList(data);"+
    // "            })"+
    // "            .catch(error => {"+
    // "                console.error(\'Error fetching time and events:\', error);"+
    // "            });"+
    "        }"+
    "        var refreshInterval = 400;"+                                     
    "        setInterval(upTime, refreshInterval);"+         
    "    </script>" +
    "    <div class=\"header\">"+
    "        <h1>ESP32-S3-POE-ETH-8DI-8RO</h1>" + 
    "    </div>" + 
    "    <nav>" + 
    "        <a href=\"/\" id=\"relayControlLink\" class=\"relayControlActive\">Relay Control</a>" +  
    "        <a href=\"/RTC_Event\" id=\"rtcEventLink\" class=\"rtcEventActive\">RTC Event</a>" +  
    "    </nav>" + 
    "    <div class=\"container\">" +  
    "        <div class=\"form-group\">" + 
    "            <label for=\"Date\">Date:(example:2024/12/20)</label>" + 
    "            <input type=\"text\" id=\"DateBox1\" style=\"width: 50px;\" value=\"2024\">" + 
    "            <span>/</span>" + 
    "            <input type=\"text\" id=\"DateBox2\" style=\"width: 50px;\" value=\"12\">" + 
    "            <span>/</span>" + 
    "            <input type=\"text\" id=\"DateBox3\" style=\"width: 50px;\" value=\"20\">" + 
    "            <span>&nbsp;&nbsp;&nbsp;</span>" + 
    "            <select id=\"Week\" style=\"width: 150px;\">" + 
    "                <option value=\"1\">星期一(Monday)</option>" + 
    "                <option value=\"2\">星期二(Tuesday)</option>" + 
    "                <option value=\"3\">星期三(Wednesday)</option>" + 
    "                <option value=\"4\">星期四(Thursday)</option>" + 
    "                <option value=\"5\">星期五(Friday)</option>" + 
    "                <option value=\"6\">星期六(Saturday)</option>" + 
    "                <option value=\"0\">星期日(Sunday)</option>" + 
    "            </select>" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <label for=\"Time\">Time:(example:16:51:21)</label>" + 
    "            <input type=\"text\" id=\"TimeBox1\" style=\"width: 50px;\" value=\"0\">" + 
    "            <span>:</span>" + 
    "            <input type=\"text\" id=\"TimeBox2\" style=\"width: 50px;\" value=\"0\">" + 
    "            <span>:</span>" + 
    "            <input type=\"text\" id=\"TimeBox3\" style=\"width: 50px;\" value=\"0\">" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <label for=\"relayBox\">Relay:</label>" +
    "            <span>CH1~CH4: </span>" + 
    "            <select id=\"RelayCH1\" style=\"width: 100px;\">" + 
    "                <option value=\"2\">保留(Retain)</option>" + 
    "                <option value=\"0\">关闭(close)</option>" + 
    "                <option value=\"1\">打开(Open)</option>" + 
    "            </select>" + 
    "            <select id=\"RelayCH2\" style=\"width: 100px;\">" + 
    "                <option value=\"2\">保留(Retain)</option>" + 
    "                <option value=\"0\">关闭(close)</option>" + 
    "                <option value=\"1\">打开(Open)</option>" + 
    "            </select>" + 
    "            <select id=\"RelayCH3\" style=\"width: 100px;\">" + 
    "                <option value=\"2\">保留(Retain)</option>" + 
    "                <option value=\"0\">关闭(close)</option>" + 
    "                <option value=\"1\">打开(Open)</option>" + 
    "            </select>" + 
    "            <select id=\"RelayCH4\" style=\"width: 100px;\">" + 
    "                <option value=\"2\">保留(Retain)</option>" + 
    "                <option value=\"0\">关闭(close)</option>" + 
    "                <option value=\"1\">打开(Open)</option>" + 
    "            </select>" + 
    "            <span><br>CH5~CH8: </span>" + 
    "            <select id=\"RelayCH5\" style=\"width: 100px;\">" + 
    "                <option value=\"2\">保留(Retain)</option>" + 
    "                <option value=\"0\">关闭(close)</option>" + 
    "                <option value=\"1\">打开(Open)</option>" + 
    "            </select>" + 
    "            <select id=\"RelayCH6\" style=\"width: 100px;\">" + 
    "                <option value=\"2\">保留(Retain)</option>" + 
    "                <option value=\"0\">关闭(close)</option>" + 
    "                <option value=\"1\">打开(Open)</option>" + 
    "            </select>" + 
    "            <select id=\"RelayCH7\" style=\"width: 100px;\">" + 
    "                <option value=\"2\">保留(Retain)</option>" + 
    "                <option value=\"0\">关闭(close)</option>" + 
    "                <option value=\"1\">打开(Open)</option>" + 
    "            </select>" + 
    "            <select id=\"RelayCH8\" style=\"width: 100px;\">" + 
    "                <option value=\"2\">保留(Retain)</option>" + 
    "                <option value=\"0\">关闭(close)</option>" + 
    "                <option value=\"1\">打开(Open)</option>" + 
    "            </select>" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <label for=\"cycleBox\">Cycle:</label>" + 
    "            <select id=\"CycleBox\" style=\"width: 150px;\">" + 
    "                <option value=\"0\">无重复(Aperiodicity)</option>" + 
    "                <option value=\"1\">每天(everyday)</option>" + 
    "                <option value=\"2\">每周(Weekly)</option>" + 
    "                <option value=\"3\">每月(monthly)</option>" + 
    "            </select>" + 
    "        </div>" + 
    "        <div class=\"form-group\">" + 
    "            <button class=\"btn\" id=\"NewEvent\" onclick=\"getRtcEventData()\">New Event</button>" + 
    "        </div>" + 
    "    </div>" + 
    "    <div class=\"container\">" +  
    "        <div class=\"form-group\">" + 
    "            <span id=\"Time\"></span> "+
    "        </div>" + 
    "        <div class=\"Events\">" + 
    "            <ul id=\"myList\"> "+
    "            </ul> "+
    "        </div>" + 
    "    </div>" + 
    "</body>" + 
    "</html>"; 

    server.send(200, "text/html", rtcPage);   
    printf("The user visited the RTC Event page\r\n"); 
}



void handleGetData() {
  String json = "[";
  for (int i = 0; i < sizeof(Relay_Flag) / sizeof(Relay_Flag[0]); i++) {
    json += String(Relay_Flag[i]);
    if (i < sizeof(Relay_Flag) / sizeof(Relay_Flag[0]) - 1) {
      json += ",";
    }
  }
  json += "]";
  server.send(200, "application/json", json);
}

void handleSwitch(uint8_t ledNumber) {
  uint8_t Data[1]={0};
  Data[0]=ledNumber+48;
  Relay_Analysis(Data,WIFI_Mode);
  server.send(200, "text/plain", "OK");
}
void handleSwitch1() { handleSwitch(1); }
void handleSwitch2() { handleSwitch(2); }
void handleSwitch3() { handleSwitch(3); }
void handleSwitch4() { handleSwitch(4); }
void handleSwitch5() { handleSwitch(5); }
void handleSwitch6() { handleSwitch(6); }
void handleSwitch7() { handleSwitch(7); }
void handleSwitch8() { handleSwitch(8); }
void handleSwitch9() { handleSwitch(9); }
void handleSwitch0() { handleSwitch(0); }

void handlePulseRelay() {
  if (server.hasArg("channel")) {
    uint8_t channel = server.arg("channel").toInt();
    uint32_t pulse_time;
    
    // Use channel's configured duration if time parameter is not provided or is empty
    if (server.hasArg("time") && server.arg("time").length() > 0) {
      pulse_time = server.arg("time").toInt();
    } else {
      // Use configured duration for the channel (channel 1-8, array index 0-7)
      if (channel >= 1 && channel <= 8) {
        pulse_time = Channel_Pulse_Durations[channel - 1];
      } else {
        server.send(400, "text/plain", "Invalid channel number");
        return;
      }
    }
    
    printf("Pulse request: CH%d, time=%dms\r\n", channel, pulse_time);
    Relay_Pulse(channel, pulse_time);
    
    server.send(200, "text/plain", "Pulse operation started");
  } else {
    server.send(400, "text/plain", "Missing channel parameter");
  }
}

void handleSetChannelPulseSettings() {
  if (server.hasArg("channel")) {
    uint8_t channel = server.arg("channel").toInt();
    
    if (channel >= 1 && channel <= 8) {
      bool updated = false;
      
      // Update pulse duration if time parameter is provided
      if (server.hasArg("time")) {
        uint32_t pulse_time = server.arg("time").toInt();
        if (pulse_time >= 10 && pulse_time <= 10000) {
          Channel_Pulse_Durations[channel - 1] = pulse_time;
          saveChannelPulseTimeToNVS(channel, pulse_time);
          printf("Pulse time set: CH%d = %dms\r\n", channel, pulse_time);
          updated = true;
        } else {
          server.send(400, "text/plain", "Invalid time value (must be 10-10000ms)");
          return;
        }
      }
      
      // Update pulse count if provided  
      if (server.hasArg("count")) {
        uint32_t pulse_count = server.arg("count").toInt();
        if (pulse_count >= 1 && pulse_count <= 10) {
          Channel_Pulse_Counts[channel - 1] = pulse_count;
          saveChannelPulseCountToNVS(channel, pulse_count);
          printf("Pulse count set: CH%d = %d cycles\r\n", channel, pulse_count);
          updated = true;
        } else {
          server.send(400, "text/plain", "Invalid count value (must be 1-10 cycles)");
          return;
        }
      }
      
      if (updated) {
        server.send(200, "text/plain", "Channel pulse settings saved");
      } else {
        server.send(400, "text/plain", "No valid time or count parameter provided");
      }
    } else {
      server.send(400, "text/plain", "Invalid channel number");
    }
  } else {
    server.send(400, "text/plain", "Missing channel parameter");
  }
}

void handleGetChannelPulseSettings() {
  String json = "{";
  json += "\"durations\":[";
  for (int i = 0; i < 8; i++) {
    json += String(Channel_Pulse_Durations[i]);
    if (i < 7) {
      json += ",";
    }
  }
  json += "],\"counts\":[";
  for (int i = 0; i < 8; i++) {
    json += String(Channel_Pulse_Counts[i]);
    if (i < 7) {
      json += ",";
    }
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleNewEvent(void) {
  char Text[200];
  if (server.hasArg("data")) {
    String newData = server.arg("data");
    newData.toCharArray(Text, sizeof(Text));
  }
  server.send(200, "text/plain", "OK");

  printf("Text=%s.\r\n",Text);  // Text=Date: 2024/12/20  Week: 0  Time: 0:0:0  Relay CH1: 0  Relay CH2: 2  Relay CH3: 2  Relay CH4: 2  Relay CH5: 2  Relay CH6: 2  Relay CH7: 2  Relay CH8: 2  Cycle: 0.
  datetime_t Event_Time;
  Repetition_event cycleEvent;
  Status_adjustment Relay_n[8];
  parseData(Text, &Event_Time, Relay_n, &cycleEvent);
  // Print decoded values
  // printf("Decoded datetime:\n");
  // printf("Year: %d, Month: %d, Day: %d, Week: %d\r\n", Event_Time.year, Event_Time.month, Event_Time.day, Event_Time.dotw);
  // printf("Time: %d:%d:%d\r\n", Event_Time.hour, Event_Time.minute, Event_Time.second);
  // printf("Relay States:\r\n");
  // for (int i = 0; i < 8; i++) {
  //   printf("Relay CH%d: %d\r\n", i + 1, Relay_n[i]);
  // }
  // printf("Cycle Event: %d\r\n", cycleEvent);
  if(Event_Time.month > 12 || Event_Time.day > 31 || Event_Time.dotw > 6 || Event_Time.month == 0 || Event_Time.day == 0)
    printf("Error parsing Event_Time !!!!\r\n");
  else if(Event_Time.hour > 23 || Event_Time.minute > 59 || Event_Time.second > 59 )
    printf("Error parsing Event_Time !!!!\r\n");
  else
    TimerEvent_CHxn_Set(Event_Time, Relay_n, cycleEvent);
}

void handleUpTimeAndEvent() {
  // Format the datetime string
  char datetime_str[50];
  sprintf(datetime_str, " %d/%d/%d  %s  %d:%d:%d", datetime.year, datetime.month, datetime.day, Week[datetime.dotw], datetime.hour, datetime.minute, datetime.second);

  int eventCount = Timing_events_Num;  // Get the event count (e.g., Timing_events_Num)

  // Create a JSON response
  String jsonResponse = "{";
  
  jsonResponse += "\"time\":\"" + String(datetime_str) + "\",";
  for (int i = 0; i < eventCount; i++) {
    jsonResponse += "\"eventStr" + String(i + 1) + "\":\"" + String(Event_str[i]) + "\",";
  }
  jsonResponse += "\"eventCount\":" + String(eventCount);
  jsonResponse += "}";
  // Send the datetime string as a response
  server.send(200, "text/plain", jsonResponse );
}
void handleDeleteEvent() {
  if (server.hasArg("id")) {
    int id = server.arg("id").toInt();
    if (id > 0) {
      TimerEvent_Del_Number((uint8_t)id);
      server.send(200, "text/plain", "Event " + String(id) + " deleted.");
      printf("Event %d deleted.\r\n", id);
    } else {
      server.send(400, "text/plain", "Invalid event ID.");
    }
  } else {
    server.send(400, "text/plain", "Event ID not provided.");
  }
}

void loadChannelPulseSettingsFromNVS() {
  preferences.begin("relay-settings", false);
  for (int i = 0; i < 8; i++) {
    String timeKey = "time" + String(i + 1);
    String countKey = "count" + String(i + 1);
    Channel_Pulse_Durations[i] = preferences.getUInt(timeKey.c_str(), 500); // Default 500ms
    Channel_Pulse_Counts[i] = preferences.getUInt(countKey.c_str(), 2); // Default 2 cycles
  }
  preferences.end();
  printf("Channel pulse settings loaded from NVS\r\n");
}

void saveChannelPulseTimeToNVS(uint8_t channel, uint32_t time) {
  preferences.begin("relay-settings", false);
  String key = "time" + String(channel);
  preferences.putUInt(key.c_str(), time);
  preferences.end();
  printf("Pulse time saved to NVS: CH%d = %dms\r\n", channel, time);
}

void saveChannelPulseDurationToNVS(uint8_t channel, uint32_t duration) {
  // Legacy function - redirect to saveChannelPulseTimeToNVS for compatibility
  saveChannelPulseTimeToNVS(channel, duration);
}

void saveChannelPulseCountToNVS(uint8_t channel, uint32_t count) {
  preferences.begin("relay-settings", false);
  String key = "count" + String(channel);
  preferences.putUInt(key.c_str(), count);
  preferences.end();
  printf("Pulse count saved to NVS: CH%d = %d cycles\r\n", channel, count);
}
void WIFI_Init()
{

  WiFi.mode(WIFI_AP);                             
  // WiFi.setSleep(true);    
  while(!WiFi.softAP(ssid, password)) {
    printf("Soft AP creation failed.\r\n");
    printf("Try setting up the WIFI again.\r\n");
  } 
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // Set the IP address and gateway of the AP
  delay(100); 
  RGB_Open_Time(0, 60, 0,1000, 0); 
  
  IPAddress myIP = WiFi.softAPIP();
  uint32_t ipAddress = WiFi.softAPIP();
  printf("AP IP address: ");
  sprintf(ipStr, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
  printf("%s\r\n", ipStr);

  // Load channel pulse settings from persistent storage
  loadChannelPulseSettingsFromNVS();

  server.on("/", handleRoot);            // Relay Control page
  server.on("/getData", handleGetData);
  server.on("/Switch1", handleSwitch1);
  server.on("/Switch2", handleSwitch2);
  server.on("/Switch3", handleSwitch3);
  server.on("/Switch4", handleSwitch4);
  server.on("/Switch5", handleSwitch5);
  server.on("/Switch6", handleSwitch6);
  server.on("/Switch7", handleSwitch7);
  server.on("/Switch8", handleSwitch8);
  server.on("/AllOn"  , handleSwitch9);
  server.on("/AllOff" , handleSwitch0);
  server.on("/PulseRelay", handlePulseRelay);  // Pulse relay endpoint
  server.on("/SetChannelPulseSettings", handleSetChannelPulseSettings);  // Set channel pulse settings
  server.on("/GetChannelPulseSettings", handleGetChannelPulseSettings);  // Get channel pulse settings
  
  server.on("/RTC_Event", handleRTCPage);      // RTC Event page
  server.on("/NewEvent" , handleNewEvent);
  server.on("/getTimeAndEvent", handleUpTimeAndEvent);
  server.on("/DeleteEvent", handleDeleteEvent);
  
  server.begin(); 
  printf("Web server started\r\n");  
  xTaskCreatePinnedToCore(
    WebTask,    
    "WebTask",   
    4096,                
    NULL,                 
    4,                   
    NULL,                 
    0                   
  );
}


void WebTask(void *parameter) {
  while(1){
    WIFI_Loop();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  vTaskDelete(NULL);
}
void WIFI_Loop()
{
  server.handleClient(); // Processing requests from clients
}

// String decoding
bool parseData(const char* Text, datetime_t* dt, Status_adjustment* Relay_n, Repetition_event* cycleEvent) {    
  int ret;
  // Parse Date: YYYY/MM/DD
  ret = sscanf(Text, "Date: %hd/%hhd/%hhd", &dt->year, &dt->month, &dt->day);
  if (ret != 3) {
    printf("Error parsing date\n");
    return false;
  }
  // Parse Week: W (day of the week)
  ret = sscanf(strstr(Text, "Week: "), "Week: %hhd", &dt->dotw);
  if (ret != 1) {
    printf("Error parsing week\n");
    return false;
  }
  // Parse Time: HH:MM:SS
  ret = sscanf(strstr(Text, "Time: "), "Time: %hhd:%hhd:%hhd", &dt->hour, &dt->minute, &dt->second);
  if (ret != 3) {
    printf("Error parsing time\n");
    return false;
  }
  // Parse Relay states (Relay CH1 to Relay CH8)
  uint8_t Relay[8];
  ret = sscanf(strstr(Text, "Relay CH"), "Relay CH1: %hhd  Relay CH2: %hhd  Relay CH3: %hhd  Relay CH4: %hhd  Relay CH5: %hhd  Relay CH6: %hhd  Relay CH7: %hhd  Relay CH8: %hhd", &Relay[0], &Relay[1], &Relay[2], &Relay[3], &Relay[4], &Relay[5], &Relay[6], &Relay[7]);
  if (ret != 8) {
    printf("Error parsing relay state\n");
    return false;
  }
  for(int i=0;i<Relay_Number_MAX;i++){
    Relay_n[i] = (Status_adjustment)Relay[i];
  }
  // Parse Cycle: C
  uint8_t cycle;
  ret = sscanf(strstr(Text, "Cycle: "), "Cycle: %hhd",&cycle);
  if (ret == 1) {
    *cycleEvent = (Repetition_event)cycle;
  }
  else{
    printf("Error parsing cycle\n");
    return false;
  }
  return true;
}




