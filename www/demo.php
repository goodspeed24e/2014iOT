<!DOCTYPE html>
<html lang="zh_tw">
<html>
<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" href="http://code.jquery.com/mobile/1.4.1/jquery.mobile-1.4.1.min.css">
	<script src="http://code.jquery.com/jquery-1.10.2.min.js"></script>
	<script src="http://code.jquery.com/mobile/1.4.1/jquery.mobile-1.4.1.min.js"></script>
</head>
<body>

<div data-role="page">

  <div data-role="header">
  <h1>智慧家庭感應器列表</h1>
  </div>  <!-- end of header -->

  <div data-role="main" class="ui-content">
    <form name="test" id="test" action="setSensorProps.php" method="post" data-transition="pop">
	<div data-role="collapsibleset">
			<fieldset data-role="collapsible" >
				<legend>一氧化碳<img src="icon/co2.png" width=20 height=20 align=left><span class="ui-li-count" id="currCO">25</span></legend>
				<label for="urgentValue_CO">危險值</label>
					<input type="range" name="urgentValue_CO" id="urgentValue_CO" value="40" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
					<label for="urgentActions_CO"></label>
					<select name="urgentActions_CO" id="urgentActions_CO" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
			<input type="submit" id= "loadCO" data-inline="true" value="確定!">
			</fieldset>
		
			<fieldset data-role="collapsible" >
				<legend>火警<img src="icon/flame.png" width=20 height=20 align=left><span class="ui-li-count" id="currFlame">5</span></legend>
				<label for="urgentValue_FLAME">危險值</label>
					<input type="range" name="urgentValue_FLAME" id="urgentValue_FLAME" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_FLAME"></label>
					<select name="危險觸發處制動作" id="urgentActions_FLAME" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
		  
		  <fieldset data-role="collapsible" >
				<legend>瓦斯<img src="icon/gas.png" width=20 height=20 align=left><span class="ui-li-count" id="currGAS" >32</span></legend>
				<label for="urgentValue_GAS">危險值</label>
					<input type="range" name="urgentValue_GAS" id="urgentValue_GAS" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_GAS"></label>
					<select name="危險觸發處制動作" id="urgentActions_GAS" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
		  
		  <fieldset data-role="collapsible" >
				<legend>小偷<img src="icon/thief.png" width=20 height=20 align=left><span class="ui-li-count" id="currThief">2</span></legend>
				<label for="urgentValue_PEOPLE">危險值</label>
					<input type="range" name="urgentValue_PEOPLE" id="urgentValue_PEOPLE" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_PEOPLE"></label>
					<select name="危險觸發處制動作" id="urgentActions_PEOPLE" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
		  
		  <fieldset data-role="collapsible" >
				<legend>漏水<img src="icon/water.png" width=20 height=20 align=left><span class="ui-li-count" id="currWater">9</span></legend>
				<label for="urgentValue_WATER">危險值</label>
					<input type="range" name="urgentValue_WATER" id="urgentValue_WATER" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_WATER"></label>
					<select name="危險觸發處制動作" id="urgentActions_WATER" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
		  
		  
		  <fieldset data-role="collapsible" >
				<legend>磁力<img src="icon/window.png" width=20 height=20 align=left><span class="ui-li-count" id="currWindow">15</span></legend>
				<label for="urgentValue_MAGETIC">危險值</label>
					<input type="range" name="urgentValue_MAGETIC" id="urgentValue_MAGETIC" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_MAGETIC"></label>
					<select name="危險觸發處制動作" id="urgentActions_MAGETIC" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
		  
		  
		  <fieldset data-role="collapsible" >
				<legend>室內溫度<img src="icon/temperature.png" width=20 height=20 align=left><span class="ui-li-count" id="currTemper" >25</span></legend>
				<label for="urgentValue_TEMPERATURE">危險值</label>
					<input type="range" name="urgentValue_TEMPERATURE" id="urgentValue_TEMPERATURE" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_TEMPERATURE"></label>
					<select name="危險觸發處制動作" id="urgentActions_TEMPERATURE" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
		  
		  
		  <fieldset data-role="collapsible" >
				<legend>噪音<img src="icon/noise.png" width=20 height=20 align=left><span class="ui-li-count" id="currNoise">14</span></legend>
				<label for="urgentValue_NOISE">危險值</label>
					<input type="range" name="urgentValue_NOISE" id="urgentValue_NOISE" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_NOISE"></label>
					<select name="危險觸發處制動作" id="urgentActions_NOISE" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
		  
		  
		  <fieldset data-role="collapsible" >
				<legend>自動感光<img src="icon/lamp.png" width=20 height=20 align=left><span class="ui-li-count" id="currLamp">6</span></legend>
				<label for="urgentValue_LIGHT">危險值</label>
					<input type="range" name="urgentValue_LIGHT" id="urgentValue_LIGHT" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_LIGHT"></label>
					<select name="危險觸發處制動作" id="urgentActions_LIGHT" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
		  
		  
		  <fieldset data-role="collapsible" >
				<legend>窗戶震動-陽台<img src="icon/window.png" width=20 height=20 align=left><span class="ui-li-count" id="currShock">2</span></legend>
				<label for="urgentValue_SHOCK">危險值</label>
					<input type="range" name="urgentValue_SHOCK" id="urgentValue_SHOCK" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_SHOCK"></label>
					<select name="危險觸發處制動作" id="urgentActions_SHOCK" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
		  
		  <fieldset data-role="collapsible" >
				<legend>大門進入<img src="icon/door.png" width=20 height=20 align=left><span class="ui-li-count" id="currDoor">22</span></legend>
				<label for="urgentValue_DOOR">危險值</label>
					<input type="range" name="urgentValue_DOOR" id="urgentValue_DOOR" value="60" min="0" max="100" data-mini="true" data-highlight="true" data-popup-enabled="true" data-track-theme="b" data-theme="a">
				<fieldset>
				<label for="urgentActions_DOOR"></label>
					<select name="危險觸發處制動作" id="urgentActions_DOOR" multiple="multiple" data-native-menu="false" data-mini="true">
					<option>危險觸發處制動作</option>
					<option value="Bee">感應器蜂鳴聲響</option>
					<option value="Alarm">家庭警報聲響</option>
					<option value="Message">手機簡訊通知</option>
					<option value="Photo">手機照片傳送</option>
					<option value="Map">家庭感應器平面配置圖</option>
					</select>
				</fieldset>
				<input type="submit" data-inline="true" value="確定!">
		  </fieldset>
	</div> <!-- end of 	collapsibleset -->
    </form> <!-- end of form -->
  </div> <!-- end of main -->

  <div data-role="footer">
        <form name="testAlarm" id ="testAlarm" action="testSensorProps.php" method="post" data-transition="pop">
        <div class="ui-field-contain">
                <label for="flip_alarm">家庭警報聲響</label>
                <select name="flip_alarm" id="flip_alarm" data-role="slider" data-mini="true">
                      	<option value="0">關閉</option>
                        <option value="1">開啟</option>
                </select>
                <br><br>
                <input type="submit" data-inline="true" value="Submit">
        </div>
        </form>
  </div> <!-- end of footer -->
  
</div> <!-- end of page -->

<script>
var myVar=setInterval(function(){querySensorStatus()},1000);
function querySensorStatus()
{
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currCO").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currFlame").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currGAS").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currThief").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currWater").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currWindow").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currTemper").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currNoise").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currLamp").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currShock").innerHTML=reponseVal;
	
	var reponseVal = Math.floor((Math.random()*30)+1);
	document.getElementById("currDoor").innerHTML=reponseVal;
}
</script>
</body>
</html>
