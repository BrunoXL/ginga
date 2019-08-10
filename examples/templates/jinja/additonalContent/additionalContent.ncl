<?xml version="1.0" encoding="UTF-8"?> 
<!-- An NCL Aditional Content example with embedded Jinja2 template -->
<ncl id="slideShow">
    <head>
        <connectorBase>
            <importBase documentURI="connectorBase.ncl" alias="conBase"/>
        </connectorBase>
		<regionBase>
			<region id="rVideo" width="100%" height="100%" zIndex="1"/>
			<region id="rBegin" width="6%" height="8%" left="90%" top="2%" zIndex="2"/>
			<region id="rExit" width="6%" height="8%" left="90%" top="2%" zIndex="2"/>			
			<region id="rReg" width="25%" height="3%" left="69%" top="10.8%" zIndex="2"/>
						
			<region id="rButton1" width="25%" height="5%" left="69%" top="73%" zIndex="2"/>
			<region id="rButton2" width="25%" height="5%" left="69%" top="78%" zIndex="2"/>
			<region id="rButton3" width="25%" height="5%" left="69%" top="83%" zIndex="2"/>
			<region id="rButton4" width="25%" height="5%" left="69%" top="88%" zIndex="2"/>
			
			<region id="rButton1Right" width="3%" height="3%" left="90%" top="73%" zIndex="4"/>
			<region id="rButton2Right" width="3%" height="3%" left="90%" top="78%" zIndex="4"/>
			<region id="rButton3Right" width="3%" height="3%" left="90%" top="83%" zIndex="4"/>
			<region id="rButton4Right" width="3%" height="3%" left="90%" top="88%" zIndex="4"/>
						
			<region id="rButton1_1" width="25%" height="5%" left="69%" top="13%" zIndex="2"/>
			<region id="rButton2_1" width="25%" height="5%" left="69%" top="18%" zIndex="2"/>
			<region id="rButton3_1" width="25%" height="5%" left="69%" top="23%" zIndex="2"/>
			<region id="rButton4_1" width="25%" height="5%" left="69%" top="28%" zIndex="2"/>
			
			<region id="rButton1Title" width="2%" height="2%" left="68%" top="75%" zIndex="3"/>
			<region id="rButton2Title" width="2%" height="2%" left="68%" top="80%" zIndex="3"/>
			<region id="rButton3Title" width="2%" height="2%" left="68%" top="85%" zIndex="3"/>
			<region id="rButton4Title" width="2%" height="2%" left="68%" top="92%" zIndex="3"/>
			
			<region id="rButton1Title1" width="2%" height="2%" left="69%" top="13%" zIndex="3"/>
			<region id="rButton2Title1" width="2%" height="2%" left="69%" top="18%" zIndex="3"/>
			<region id="rButton3Title1" width="2%" height="2%" left="69%" top="23%" zIndex="3"/>
			<region id="rButton4Title1" width="2%" height="2%" left="69%" top="28%" zIndex="3"/>
			
			<region id="rButton1Bg" width="25%" height="60%" left="69%" top="18%" zIndex="2"/>
			<region id="rButton2Bg" width="25%" height="60%" left="69%" top="23%" zIndex="2"/>
			<region id="rButton3Bg" width="25%" height="60%" left="69%" top="28%" zIndex="2"/>
			<region id="rButton4Bg" width="25%" height="60%" left="69%" top="33%" zIndex="2"/>
			
			<region id="rButton1Info" width="25%" height="60%" left="69.5%" top="18.8%" zIndex="3"/>
			<region id="rButton2Info" width="25%" height="60%" left="69.5%" top="23.8%" zIndex="3"/>
			<region id="rButton3Info" width="25%" height="60%" left="69.5%" top="28.8%" zIndex="3"/>
			<region id="rButton4Info" width="25%" height="60%" left="69.5%" top="33.8%" zIndex="3"/>			
		</regionBase>		
		<descriptorBase>
			<descriptor id="desVideo" region="rVideo" />
			<descriptor id="desBegin" region="rBegin"/>
			<descriptor id="desExit" region="rExit"/>	
			<descriptor id="desReg" region="rReg">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>			

			<descriptor id="desButton1" region="rButton1">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton2" region="rButton2">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton3" region="rButton3">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton4" region="rButton4">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>		

			<descriptor id="desButton1Right" region="rButton1Right">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton2Right" region="rButton2Right">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton3Right" region="rButton3Right">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton4Right" region="rButton4Right">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>		

			<descriptor id="desButton1_1" region="rButton1_1">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton2_1" region="rButton2_1">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton3_1" region="rButton3_1">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton4_1" region="rButton4_1">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>		

			<descriptor id="desButton1Bg" region="rButton1Bg">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton2Bg" region="rButton2Bg">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton3Bg" region="rButton3Bg">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>
			<descriptor id="desButton4Bg" region="rButton4Bg">
				<descriptorParam name="transparency" value="50%"/>
			</descriptor>		

			<descriptor id="desButton1Info" region="rButton1Info"/>
			<descriptor id="desButton2Info" region="rButton2Info"/>
			<descriptor id="desButton3Info" region="rButton3Info"/>
			<descriptor id="desButton4Info" region="rButton4Info"/>

			<descriptor id="desButton1Info1" region="rButton1Info"/>
			<descriptor id="desButton2Info1" region="rButton2Info"/>
			<descriptor id="desButton3Info1" region="rButton3Info"/>
			<descriptor id="desButton4Info1" region="rButton4Info"/>

			<descriptor id="desButton1Title" region="rButton1Title"/>
			<descriptor id="desButton2Title" region="rButton2Title"/>
			<descriptor id="desButton3Title" region="rButton3Title"/>
			<descriptor id="desButton4Title" region="rButton4Title"/>

			<descriptor id="desButton1Title1" region="rButton1Title1"/>
			<descriptor id="desButton2Title1" region="rButton2Title1"/>
			<descriptor id="desButton3Title1" region="rButton3Title1"/>
			<descriptor id="desButton4Title1" region="rButton4Title1"/>
				
		</descriptorBase>
	</head>
    <body>
		<port id="Inicio" component="mVideo"/>
    	<media id="mVideo" src="media/video.mp4" descriptor="desVideo">
			<property name="bounds"/>
    	</media>

		<media id="mBegin" descriptor="desBegin" src="media/info.png"/>
		<media id="mExit" descriptor="desExit" src="media/salir.png"/>
		
		<media id="mReg" descriptor="desReg" src="media/reg.png"/>

		<media id="mRed" src ="media/rojo.png" descriptor="desButton1"/>
    	<media id="mRed1" src ="media/rojo.png" descriptor="desButton1_1"/>
		<media id="mRedRight" src="media/dere.png" descriptor="desButton1Right"/> 
		<media id="mRedLeft" src="media/izq.png" descriptor="desButton1Right"/> 
		<media id="mRedBg" src ="media/fondo.png" descriptor="desButton1Bg"/>
		<media id="mRedTitle" src="media/tituloRojo.txt" descriptor="desButton1Title">
			<property name="width" value="20%"/>
			<property name="height" value="5%"/>
			<property name="left" value="74%"/>
			<property name="top" value="73.8%"/>
			<property name="fontColor" value="white"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/>
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mRedTitle1" src="media/tituloRojo.txt" descriptor="desButton1Title1">
			<property name="width" value="20%"/>
			<property name="height" value="5%"/>
			<property name="left" value="74%"/>
			<property name="top" value="13.8%"/>
			<property name="fontColor" value="white"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mRedInfo" src="media/infoRojo1.txt" descriptor="desButton1Info">
			<property name="width" value="25%"/>
			<property name="height" value="60%"/>
			<property name="left" value="70%"/>
			<property name="top" value="19%"/>
			<property name="fontColor" value="black"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mRedInfo1" src="media/infoRojo2.txt" descriptor="desButton1Info1">
			<property name="width" value="25%"/>
			<property name="height" value="60%"/>
			<property name="left" value="70%"/>
			<property name="top" value="19%"/>
			<property name="fontColor" value="black"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media> 

		<media id="mGreen" src ="media/verde.png" descriptor="desButton2"/>
    	<media id="mGreen1" src ="media/verde.png" descriptor="desButton2_1"/>
		<media id="mGreenRight" src="media/dere.png" descriptor="desButton2Right"/> 
		<media id="mGreenLeft" src="media/izq.png" descriptor="desButton2Right"/> 
		<media id="mGreenBg" src ="media/fondo.png" descriptor="desButton2Bg"/>
		<media id="mGreenTitle" src="media/tituloVerde.txt" descriptor="desButton2Title">
			<property name="width" value="20%"/>
			<property name="height" value="5%"/>
			<property name="left" value="74%"/>
			<property name="top" value="78.8%"/>
			<property name="fontColor" value="white"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/>
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mGreenTitle1" src="media/tituloVerde.txt" descriptor="desButton2Title1">
			<property name="width" value="20%"/>
			<property name="height" value="5%"/>
			<property name="left" value="74%"/>
			<property name="top" value="18.8%"/>
			<property name="fontColor" value="white"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mGreenInfo" src="media/infoVerde1.txt" descriptor="desButton2Info">
			<property name="width" value="25%"/>
			<property name="height" value="60%"/>
			<property name="left" value="70%"/>
			<property name="top" value="24%"/>
			<property name="fontColor" value="black"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mGreenInfo1" src="media/infoVerde2.txt" descriptor="desButton2Info1">
			<property name="width" value="25%"/>
			<property name="height" value="60%"/>
			<property name="left" value="70%"/>
			<property name="top" value="24%"/>
			<property name="fontColor" value="black"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media> 

		<media id="mYellow" src ="media/amarillo.png" descriptor="desButton3"/>
    	<media id="mYellow1" src ="media/amarillo.png" descriptor="desButton3_1"/>
		<media id="mYellowRight" src="media/dere.png" descriptor="desButton3Right"/> 
		<media id="mYellowLeft" src="media/izq.png" descriptor="desButton3Right"/> 
		<media id="mYellowBg" src ="media/fondo.png" descriptor="desButton3Bg"/>
		<media id="mYellowTitle" src="media/tituloAmar.txt" descriptor="desButton3Title">
			<property name="width" value="20%"/>
			<property name="height" value="5%"/>
			<property name="left" value="74%"/>
			<property name="top" value="83.8%"/>
			<property name="fontColor" value="white"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/>
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mYellowTitle1" src="media/tituloAmar.txt" descriptor="desButton3Title1">
			<property name="width" value="20%"/>
			<property name="height" value="5%"/>
			<property name="left" value="74%"/>
			<property name="top" value="23.8%"/>
			<property name="fontColor" value="white"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mYellowInfo" src="media/infoAma1.txt" descriptor="desButton3Info">
			<property name="width" value="25%"/>
			<property name="height" value="60%"/>
			<property name="left" value="70%"/>
			<property name="top" value="29%"/>
			<property name="fontColor" value="black"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mYellowInfo1" src="media/infoAma2.txt" descriptor="desButton3Info1">
			<property name="width" value="25%"/>
			<property name="height" value="60%"/>
			<property name="left" value="70%"/>
			<property name="top" value="29%"/>
			<property name="fontColor" value="black"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media> 

		<media id="mBlue" src ="media/azul.png" descriptor="desButton4"/>
    	<media id="mBlue1" src ="media/azul.png" descriptor="desButton4_1"/>
		<media id="mBlueRight" src="media/dere.png" descriptor="desButton4Right"/> 
		<media id="mBlueLeft" src="media/izq.png" descriptor="desButton4Right"/> 
		<media id="mBlueBg" src ="media/fondo.png" descriptor="desButton4Bg"/>
		<media id="mBlueTitle" src="media/tituloAzul.txt" descriptor="desButton4Title">
			<property name="width" value="20%"/>
			<property name="height" value="5%"/>
			<property name="left" value="74%"/>
			<property name="top" value="88.8%"/>
			<property name="fontColor" value="white"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/>
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mBlueTitle1" src="media/tituloAzul.txt" descriptor="desButton4Title1">
			<property name="width" value="20%"/>
			<property name="height" value="5%"/>
			<property name="left" value="74%"/>
			<property name="top" value="28.8%"/>
			<property name="fontColor" value="white"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mBlueInfo" src="media/foto1.jpg" descriptor="desButton4Info">
			<property name="width" value="23%"/>
			<property name="height" value="53%"/>
			<property name="left" value="70%"/>
			<property name="top" value="34%"/>
			<property name="fontColor" value="black"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media>
		<media id="mBlueInfo1" src="media/foto2.jpg" descriptor="desButton4Info1">
			<property name="width" value="23%"/>
			<property name="height" value="53%"/>
			<property name="left" value="70%"/>
			<property name="top" value="34%"/>
			<property name="fontColor" value="black"/>
			<property name="fontSize" value="17"/>
			<property name="zIndex" value="3"/> 
			<property name="fontFamily" value="Tiresias"/>
		</media> 

		<link xconnector="conBase#onBeginStartN">
			<bind role="onBegin" component="mVideo"/>
			<bind role="start" component="mBegin"/>
		</link>

		<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mBegin"/>
			<linkParam name="keyCode" value="INFO"/>
			<bind role="start" component="mRed"/>
			<bind role="start" component="mRedTitle"/>
		
			<bind role="start" component="mGreen"/>
			<bind role="start" component="mGreenTitle"/>
		
			<bind role="start" component="mYellow"/>
			<bind role="start" component="mYellowTitle"/>
		
			<bind role="start" component="mBlue"/>
			<bind role="start" component="mBlueTitle"/>
		
			<bind role="start" component="mExit"/>	
			<bind role="stop" component="mBegin"/>
	
		</link>
		<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mExit"/>
			<linkParam name="keyCode" value="INFO"/>
			<bind role="start" component="mBegin"/>
			<bind role="stop" component="mRed"/>
			<bind role="stop" component="mRed1"/>
			<bind role="stop" component="mRedTitle"/>
			<bind role="stop" component="mRedTitle1"/>
			<bind role="stop" component="mRedBg"/>
			<bind role="stop" component="mRedInfo"/>
			<bind role="stop" component="mRedRight"/>
			<bind role="stop" component="mRedInfo1"/>
		
			<bind role="stop" component="mGreen"/>
			<bind role="stop" component="mGreen1"/>
			<bind role="stop" component="mGreenTitle"/>
			<bind role="stop" component="mGreenTitle1"/>
			<bind role="stop" component="mGreenBg"/>
			<bind role="stop" component="mGreenInfo"/>
			<bind role="stop" component="mGreenRight"/>
			<bind role="stop" component="mGreenInfo1"/>
		
			<bind role="stop" component="mYellow"/>
			<bind role="stop" component="mYellow1"/>
			<bind role="stop" component="mYellowTitle"/>
			<bind role="stop" component="mYellowTitle1"/>
			<bind role="stop" component="mYellowBg"/>
			<bind role="stop" component="mYellowInfo"/>
			<bind role="stop" component="mYellowRight"/>
			<bind role="stop" component="mYellowInfo1"/>
		
			<bind role="stop" component="mBlue"/>
			<bind role="stop" component="mBlue1"/>
			<bind role="stop" component="mBlueTitle"/>
			<bind role="stop" component="mBlueTitle1"/>
			<bind role="stop" component="mBlueBg"/>
			<bind role="stop" component="mBlueInfo"/>
			<bind role="stop" component="mBlueRight"/>
			<bind role="stop" component="mBlueInfo1"/>
		
			<bind role="stop" component="mExit"/>	
			<bind role="stop" component="mReg"/>
	
		</link>
		<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mReg"/>
			<linkParam name="keyCode" value="CURSOR_DOWN"/>
			<bind role="stop" component="mReg"/>
			<bind role="stop" component="mRed1"/>
			<bind role="stop" component="mRedTitle1"/>
			<bind role="stop" component="mRedBg"/>
			<bind role="stop" component="mRedInfo"/>
					
			<bind role="stop" component="mGreen1"/>
			<bind role="stop" component="mGreenTitle1"/>
			<bind role="stop" component="mGreenBg"/>
			<bind role="stop" component="mGreenInfo"/>
					
			<bind role="stop" component="mYellow1"/>
			<bind role="stop" component="mYellowTitle1"/>
			<bind role="stop" component="mYellowBg"/>
			<bind role="stop" component="mYellowInfo"/>
					
			<bind role="stop" component="mBlue1"/>
			<bind role="stop" component="mBlueTitle1"/>
			<bind role="stop" component="mBlueBg"/>
			<bind role="stop" component="mBlueInfo"/>
					
			<bind role="start" component="mRed"/>
			<bind role="start" component="mRedTitle"/>
					
			<bind role="start" component="mGreen"/>
			<bind role="start" component="mGreenTitle"/>
					
			<bind role="start" component="mYellow"/>
			<bind role="start" component="mYellowTitle"/>
					
			<bind role="start" component="mBlue"/>
			<bind role="start" component="mBlueTitle"/>
					
	
		</link>
					<link xconnector="conBase#onKeySelectionStopNStartN">
			<bind role="onSelection" component="mRed"/>
			<linkParam name="keyCode" value="RED"/>
						<bind role="stop" component="mRed"/>
			<bind role="stop" component="mRedTitle"/>
			<bind role="start" component="mReg"/>
			<bind role="start" component="mRed1"/>
			<bind role="start" component="mRedTitle1"/>
	
			<bind role="start" component="mRedBg"/>
			<bind role="start" component="mRedInfo"/>	
			
		</link>
				<link xconnector="conBase#onKeySelectionStopNStartN">
			<bind role="onSelection" component="mGreen"/>
			<linkParam name="keyCode" value="GREEN"/>
						<bind role="stop" component="mRed"/>
			<bind role="stop" component="mRedTitle"/>
			<bind role="stop" component="mGreen"/>
			<bind role="stop" component="mGreenTitle"/>
			<bind role="stop" component="mRedBg"/>
			<bind role="stop" component="mRedInfo"/>
			<bind role="start" component="mReg"/>
			<bind role="start" component="mRed1"/>
			<bind role="start" component="mRedTitle1"/>
			<bind role="start" component="mGreen1"/>
			<bind role="start" component="mGreenTitle1"/>
			
			<bind role="start" component="mGreenBg"/>
			<bind role="start" component="mGreenInfo"/>	
			
		</link>
				<link xconnector="conBase#onKeySelectionStopNStartN">
			<bind role="onSelection" component="mYellow"/>
			<linkParam name="keyCode" value="YELLOW"/>
						<bind role="stop" component="mRed"/>
			<bind role="stop" component="mRedTitle"/>
			<bind role="stop" component="mGreen"/>
			<bind role="stop" component="mGreenTitle"/>
			<bind role="stop" component="mRedBg"/>
			<bind role="stop" component="mRedInfo"/>
			<bind role="stop" component="mYellow"/>
			<bind role="stop" component="mYellowTitle"/>
			<bind role="stop" component="mGreenBg"/>
			<bind role="stop" component="mGreenInfo"/>
			<bind role="start" component="mReg"/>
			<bind role="start" component="mRed1"/>
			<bind role="start" component="mRedTitle1"/>
			<bind role="start" component="mGreen1"/>
			<bind role="start" component="mGreenTitle1"/>
			<bind role="start" component="mYellow1"/>
			<bind role="start" component="mYellowTitle1"/>
			
			<bind role="start" component="mYellowBg"/>
			<bind role="start" component="mYellowInfo"/>	
			
		</link>
				<link xconnector="conBase#onKeySelectionStopNStartN">
			<bind role="onSelection" component="mBlue"/>
			<linkParam name="keyCode" value="BLUE"/>
						<bind role="stop" component="mRed"/>
			<bind role="stop" component="mRedTitle"/>
			<bind role="stop" component="mGreen"/>
			<bind role="stop" component="mGreenTitle"/>
			<bind role="stop" component="mRedBg"/>
			<bind role="stop" component="mRedInfo"/>
			<bind role="stop" component="mYellow"/>
			<bind role="stop" component="mYellowTitle"/>
			<bind role="stop" component="mGreenBg"/>
			<bind role="stop" component="mGreenInfo"/>
			<bind role="stop" component="mBlue"/>
			<bind role="stop" component="mBlueTitle"/>
			<bind role="stop" component="mYellowBg"/>
			<bind role="stop" component="mYellowInfo"/>
			<bind role="start" component="mReg"/>
			<bind role="start" component="mRed1"/>
			<bind role="start" component="mRedTitle1"/>
			<bind role="start" component="mGreen1"/>
			<bind role="start" component="mGreenTitle1"/>
			<bind role="start" component="mYellow1"/>
			<bind role="start" component="mYellowTitle1"/>
			<bind role="start" component="mBlue1"/>
			<bind role="start" component="mBlueTitle1"/>
			
			<bind role="start" component="mBlueBg"/>
			<bind role="start" component="mBlueInfo"/>	
			
		</link>

						<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mYellow1"/>
			<linkParam name="keyCode" value="YELLOW"/>
						<bind role="stop" component="mBlue1"/>
			<bind role="stop" component="mBlueTitle1"/>
			<bind role="stop" component="mBlueBg"/>
			<bind role="stop" component="mBlueInfo"/>

			<bind role="start" component="mYellowBg"/>
			<bind role="start" component="mYellowInfo"/>
			<bind role="start" component="mBlue"/>
			<bind role="start" component="mBlueTitle"/>
	
	
		</link>
				<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mGreen1"/>
			<linkParam name="keyCode" value="GREEN"/>
						<bind role="stop" component="mBlue1"/>
			<bind role="stop" component="mBlueTitle1"/>
			<bind role="stop" component="mBlueBg"/>
			<bind role="stop" component="mBlueInfo"/>
			<bind role="stop" component="mYellow1"/>
			<bind role="stop" component="mYellowTitle1"/>
			<bind role="stop" component="mYellowBg"/>
			<bind role="stop" component="mYellowInfo"/>

			<bind role="start" component="mGreenBg"/>
			<bind role="start" component="mGreenInfo"/>
			<bind role="start" component="mBlue"/>
			<bind role="start" component="mBlueTitle"/>
			<bind role="start" component="mYellow"/>
			<bind role="start" component="mYellowTitle"/>
	
	
		</link>
				<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mRed1"/>
			<linkParam name="keyCode" value="RED"/>
						<bind role="stop" component="mBlue1"/>
			<bind role="stop" component="mBlueTitle1"/>
			<bind role="stop" component="mBlueBg"/>
			<bind role="stop" component="mBlueInfo"/>
			<bind role="stop" component="mYellow1"/>
			<bind role="stop" component="mYellowTitle1"/>
			<bind role="stop" component="mYellowBg"/>
			<bind role="stop" component="mYellowInfo"/>
			<bind role="stop" component="mGreen1"/>
			<bind role="stop" component="mGreenTitle1"/>
			<bind role="stop" component="mGreenBg"/>
			<bind role="stop" component="mGreenInfo"/>
			<bind role="stop" component="mRed"/>
			<bind role="stop" component="mRedTitle"/>
			<bind role="start" component="mRedBg"/>
			<bind role="start" component="mRedInfo"/>
			<bind role="start" component="mBlue"/>
			<bind role="start" component="mBlueTitle"/>
			<bind role="start" component="mYellow"/>
			<bind role="start" component="mYellowTitle"/>
			<bind role="start" component="mGreen"/>
			<bind role="start" component="mGreenTitle"/>
	
	
		</link>

				<link xconnector="conBase#onBeginStartNStopN">
			<bind role="onBegin" component="mRedBg"/>
			<linkParam name="keyCode" value="RED"/>
					<bind role="start" component="mRedRight"/>
			<bind role="stop" component="mGreenRight"/>
			<bind role="stop" component="mYellowRight"/>
			<bind role="stop" component="mBlueRight"/>
			<bind role="stop" component="mGreenLeft"/>
			<bind role="stop" component="mYellowLeft"/>
			<bind role="stop" component="mBlueLeft"/>
			<bind role="stop" component="mGreenInfo1"/>
			<bind role="stop" component="mYellowInfo1"/>
			<bind role="stop" component="mBlueInfo1"/>

	
		</link>

				<link xconnector="conBase#onBeginStartNStopN">
			<bind role="onBegin" component="mGreenBg"/>
			<linkParam name="keyCode" value="GREEN"/>
					<bind role="start" component="mGreenRight"/>
			<bind role="stop" component="mRedRight"/>
			<bind role="stop" component="mYellowRight"/>
			<bind role="stop" component="mBlueRight"/>
			<bind role="stop" component="mRedLeft"/>
			<bind role="stop" component="mYellowLeft"/>
			<bind role="stop" component="mBlueLeft"/>
			<bind role="stop" component="mRedInfo1"/>
			<bind role="stop" component="mYellowInfo1"/>
			<bind role="stop" component="mBlueInfo1"/>

	
		</link>

				<link xconnector="conBase#onBeginStartNStopN">
			<bind role="onBegin" component="mYellowBg"/>
			<linkParam name="keyCode" value="YELLOW"/>
					<bind role="start" component="mYellowRight"/>
			<bind role="stop" component="mRedRight"/>
			<bind role="stop" component="mGreenRight"/>
			<bind role="stop" component="mBlueRight"/>
			<bind role="stop" component="mRedLeft"/>
			<bind role="stop" component="mGreenLeft"/>
			<bind role="stop" component="mBlueLeft"/>
			<bind role="stop" component="mRedInfo1"/>
			<bind role="stop" component="mGreenInfo1"/>
			<bind role="stop" component="mBlueInfo1"/>

	
		</link>

				<link xconnector="conBase#onBeginStartNStopN">
			<bind role="onBegin" component="mBlueBg"/>
			<linkParam name="keyCode" value="BLUE"/>
					<bind role="start" component="mBlueRight"/>
			<bind role="stop" component="mRedRight"/>
			<bind role="stop" component="mGreenRight"/>
			<bind role="stop" component="mYellowRight"/>
			<bind role="stop" component="mRedLeft"/>
			<bind role="stop" component="mGreenLeft"/>
			<bind role="stop" component="mYellowLeft"/>
			<bind role="stop" component="mRedInfo1"/>
			<bind role="stop" component="mGreenInfo1"/>
			<bind role="stop" component="mYellowInfo1"/>

	
		</link>

			<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mRedRight"/>
			<linkParam name="keyCode" value="CURSOR_RIGHT"/>
						<bind role="stop" component="mRedInfo"/>
			<bind role="stop" component="mRedRight"/>

			<bind role="start" component="mRedInfo1"/>
			<bind role="start" component="mRedLeft"/>
				
	
		</link>
			<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mGreenRight"/>
			<linkParam name="keyCode" value="CURSOR_RIGHT"/>
						<bind role="stop" component="mGreenInfo"/>
			<bind role="stop" component="mGreenRight"/>

			<bind role="start" component="mGreenInfo1"/>
			<bind role="start" component="mGreenLeft"/>
				
	
		</link>
			<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mYellowRight"/>
			<linkParam name="keyCode" value="CURSOR_RIGHT"/>
						<bind role="stop" component="mYellowInfo"/>
			<bind role="stop" component="mYellowRight"/>

			<bind role="start" component="mYellowInfo1"/>
			<bind role="start" component="mYellowLeft"/>
				
	
		</link>
			<link xconnector="conBase#onKeySelectionStartNStopN">
			<bind role="onSelection" component="mBlueRight"/>
			<linkParam name="keyCode" value="CURSOR_RIGHT"/>
						<bind role="stop" component="mBlueInfo"/>
			<bind role="stop" component="mBlueRight"/>

			<bind role="start" component="mBlueInfo1"/>
			<bind role="start" component="mBlueLeft"/>
				
	
		</link>
		<link xconnector="conBase#onKeySelectionStopNStartN">
			<bind role="onSelection" component="mRedLeft"/>
			<linkParam name="keyCode" value="CURSOR_LEFT"/>
						<bind role="stop" component="mRedInfo1"/>
			<bind role="stop" component="mRedLeft"/>

			<bind role="start" component="mRedInfo"/>
			<bind role="start" component="mRedRight"/>
				
	
		</link>
		<link xconnector="conBase#onKeySelectionStopNStartN">
			<bind role="onSelection" component="mGreenLeft"/>
			<linkParam name="keyCode" value="CURSOR_LEFT"/>
						<bind role="stop" component="mGreenInfo1"/>
			<bind role="stop" component="mGreenLeft"/>

			<bind role="start" component="mGreenInfo"/>
			<bind role="start" component="mGreenRight"/>
				
	
		</link>
		<link xconnector="conBase#onKeySelectionStopNStartN">
			<bind role="onSelection" component="mYellowLeft"/>
			<linkParam name="keyCode" value="CURSOR_LEFT"/>
						<bind role="stop" component="mYellowInfo1"/>
			<bind role="stop" component="mYellowLeft"/>

			<bind role="start" component="mYellowInfo"/>
			<bind role="start" component="mYellowRight"/>
				
	
		</link>
		<link xconnector="conBase#onKeySelectionStopNStartN">
			<bind role="onSelection" component="mBlueLeft"/>
			<linkParam name="keyCode" value="CURSOR_LEFT"/>
						<bind role="stop" component="mBlueInfo1"/>
			<bind role="stop" component="mBlueLeft"/>

			<bind role="start" component="mBlueInfo"/>
			<bind role="start" component="mBlueRight"/>
				
	
		</link>
				<link xconnector="conBase#onBeginStopN">
			<bind role="onBegin" component="mRed"/>
			<linkParam name="keyCode" value="RED"/>
						<bind role="stop" component="mRedLeft"/>
			<bind role="stop" component="mRedRight"/>
			<bind role="stop" component="mRedInfo1"/>
				
	
		</link>
			<link xconnector="conBase#onBeginStopN">
			<bind role="onBegin" component="mGreen"/>
			<linkParam name="keyCode" value="GREEN"/>
						<bind role="stop" component="mGreenLeft"/>
			<bind role="stop" component="mGreenRight"/>
			<bind role="stop" component="mGreenInfo1"/>
				
	
		</link>
			<link xconnector="conBase#onBeginStopN">
			<bind role="onBegin" component="mYellow"/>
			<linkParam name="keyCode" value="YELLOW"/>
						<bind role="stop" component="mYellowLeft"/>
			<bind role="stop" component="mYellowRight"/>
			<bind role="stop" component="mYellowInfo1"/>
				
	
		</link>
			<link xconnector="conBase#onBeginStopN">
			<bind role="onBegin" component="mBlue"/>
			<linkParam name="keyCode" value="BLUE"/>
						<bind role="stop" component="mBlueLeft"/>
			<bind role="stop" component="mBlueRight"/>
			<bind role="stop" component="mBlueInfo1"/>
				
	
		</link>

	</body>
</ncl>