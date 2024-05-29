const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>Servo Driver with ESP32 by Kevin Stark</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <link rel="icon" href="data:,">
    <style>
        html {
        font-family: Arial;
        display: inline-block;
        background: #000000;
        color: #efefef;
        text-align: center;
    }

    h2 {
        font-size: 3.0rem;
    }

    p {
        font-size: 1.0rem;
    }

    body {
        max-width: 600px;
        margin: 0px auto;
        padding-bottom: 25px;
    }

    button {
        display: inline-block;
        margin: 5px;
        padding: 10px 10px;
        border: 0;
        line-height: 21px;
        cursor: pointer;
        color: #fff;
        background: #4247b7;
        border-radius: 5px;
        font-size: 21px;
        outline: 0;
        width: 100px

        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;

        user-select: none;
    }

    button:hover {
        background: #ff494d
    }

    button:active {
        background: #f21c21
    }

    </style>
</head>

<body>
    <h3>A Compact and Lightweight Rigid-Tendon Combined Exoskeleton for Hand Rehabilitation</h3>
    <p>https://github.com/ksDreamer/hand_exoskeleton</p>
    <p>
    <span id="IDValue">Click this button to start searching servos.</span>
    <p>
    <label align="center"><button class="button" onclick="toggleCheckbox(9, 0, 0, 0);">Start Searching</button></label>
    <p>
    <span id="STSValue">Single servo infomation.</span>
    <p>
        <!--Combined Action-->
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 23, 0, 0);">23 Open Hand</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 24, 0, 0);">24 Close Hand</button></label> 
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 25, 0, 0);">25 Open and Close</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 26, 0, 0);">26 Show Motion</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 27, 0, 0);">27 Yeah</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 28, 0, 0);">28 Play Game</button></label>

    <p>
        <!-- Thumb's Action -->
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 29, 0, 0);">29 Thumb 紧贴手背</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 30, 0, 0);">30 Thumb 远离手背</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 31, 0, 0);">31 Thumb 张开</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 32, 0, 0);">32 Thumb 收紧</button></label>

    <p>
        <!-- Finger's Action-->
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 33, 0, 0);">33 Finger 2 Move</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 34, 0, 0);">34 Finger 2 Open</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 35, 0, 0);">35 Finger 2 Close</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 36, 0, 0);">36 Finger 3 Move</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 37, 0, 0);">37 Finger 3 Open</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 38, 0, 0);">38 Finger 3 Close</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 39, 0, 0);">39 Finger 4 Move</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 40, 0, 0);">40 Finger 4 Open</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 41, 0, 0);">41 Finger 4 Close</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 42, 0, 0);">42 Finger 5 Move</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 43, 0, 0);">43 Finger 5 Open</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 44, 0, 0);">44 Finger 5 Close</button></label>

    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 45, 0, 0);">45 Speed Set to 100</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 46, 0, 0);">46 Speed Set to 700</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 47, 0, 0);">47 Speed Set to 1500</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(0, 1, 0, 0);">ID Select+</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(0, -1, 0, 0);">ID Select-</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 9, 0, 0);">ID to Set+</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 10, 0, 0);">ID to Set-</button></label>
    <!--
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 1, 0, 0);">Middle</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 2, 0, 0);">Stop</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 3, 0, 0);">Release</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 4, 0, 0);">Torque</button></label>
    <p>
        <label align="center"><button class="button" onmousedown="toggleCheckbox(1, 5, 0, 0);" ontouchstart="toggleCheckbox(1, 5, 0, 0);" onmouseup="toggleCheckbox(1, 2, 0, 0);" ontouchend="toggleCheckbox(1, 2, 0, 0);">Position+</button></label>
        <label align="center"><button class="button" onmousedown="toggleCheckbox(1, 6, 0, 0);" ontouchstart="toggleCheckbox(1, 6, 0, 0);" onmouseup="toggleCheckbox(1, 2, 0, 0);" ontouchend="toggleCheckbox(1, 2, 0, 0);">Position-</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 7, 0, 0);">Speed+</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 8, 0, 0);">Speed-</button></label>
    
    <p>
        <label align="center"><button class="button" onclick="setMiddle();">Set Middle Position</button></label>
        <label align="center"><button class="button" onclick="setNewID();">Set New ID</button></label>
    <p>
        <label align="center"><button class="button" onclick="setServoMode();">Set Servo Mode</button></label>
        <label align="center"><button class="button" onclick="setStepperMode();">Set Motor Mode</button></label>
    <p>
        <label align="center"><button class="button" id="serialForwarding" onclick="serialForwarding();">Start Serial Forwarding</button></label>
    <p>
        <label align="center"><button class="button" onclick="setRole(0);">Normal</button></label>
        <label align="center"><button class="button" onclick="setRole(1);">Leader</button></label>
        <label align="center"><button class="button" onclick="setRole(2);">Follower</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 20, 0, 0);">RainbowON</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 21, 0, 0);">RainbowOFF</button></label>
    -->
    <script>
        serialForwardStatus = false;

        function toggleCheckbox(inputT, inputI, inputA, inputB) {
            console.log('toggleCheckbox start'); 
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "cmd?inputT="+inputT+"&inputI="+inputI+"&inputA="+inputA+"&inputB="+inputB, true);
            xhr.send();
            console.log('toggleCheckbox end'); 
        }

        function ctrlMode() {
            xhr.open("GET", "ctrl", true);
            xhr.send();
        }

        setInterval(function() {
          getData();
        }, 300);

        setInterval(function() {
          getServoID();
        }, 1500);

        function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("STSValue").innerHTML =
                  this.responseText;
                }
            };
            xhttp.open("GET", "readSTS", true);
            xhttp.send();
        }

        function getServoID() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("IDValue").innerHTML =
                  this.responseText;
                }
            };
            xhttp.open("GET", "readID", true);
            xhttp.send();
        }

        function setRole(modeNum){
            if(modeNum == 0){
                var r=confirm("Set the role as Normal. Dev won't send or receive data via ESP-NOW.");
                if(r==true){
                    toggleCheckbox(1, 17, 0, 0);
                }
            }
            if(modeNum == 1){
                var r=confirm("Set the role as Leader. Dev will send data via ESP-NOW.");
                if(r==true){
                    toggleCheckbox(1, 18, 0, 0);
                }
            }
            if(modeNum == 2){
                var r=confirm("Set the role as Follower. Dev will receive data via ESP-NOW.");
                if(r==true){
                    toggleCheckbox(1, 19, 0, 0);
                }
            }
        }

        function setMiddle(){
            var r=confirm("The middle position of the active servo will be set.");
            if(r==true){
                toggleCheckbox(1, 11, 0, 0);
            }
        }

        function setServoMode(){
            var r=confirm("The active servo will be set as servoMode.");
            if(r==true){
                toggleCheckbox(1, 12, 0, 0);
            }
        }

        function setStepperMode(){
            var r=confirm("The active servo will be set as motorMode.");
            if(r==true){
                toggleCheckbox(1, 13, 0, 0);
            }
        }

        function setNewID(){
            var r=confirm("A new ID of the active servo will be set.");
            if(r==true){
                toggleCheckbox(1, 16, 0, 0);
            }
        }

        function serialForwarding(){
            if(!serialForwardStatus){
                var r=confirm("Do you want to start serial forwarding?");
                if(r){
                    toggleCheckbox(1, 14, 0, 0);
                    serialForwardStatus = true;
                    document.getElementById("serialForwarding").innerHTML = "Stop Serial Forwarding";
                }
            }
            else{
                var r=confirm("Do you want to stop serial forwarding?");
                if(r){
                    toggleCheckbox(1, 15, 0, 0);
                    serialForwardStatus = false;
                    document.getElementById("serialForwarding").innerHTML = "Start Serial Forwarding";
                }
            }
        }

    </script>
</body>
</html>
)rawliteral";
