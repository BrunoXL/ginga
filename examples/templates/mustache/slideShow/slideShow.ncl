<?xml version="1.0" encoding="UTF-8"?> 
<!-- An NCL SlideShow example with embedded mustache template -->
<ncl id="slideShow">
    <head>
        <connectorBase>
            <importBase documentURI="connectorBase.ncl" alias="conBase"/>
        </connectorBase>
        <regionBase>
            <region id="main" width="100%" height="100%" zIndex="1"/>
        </regionBase>
        <descriptorBase>
            <descriptor id="ImageDes" region="main" explicitDur="5s"/>
        </descriptorBase>
    </head>
    <body>
        <port id="startSlideShow" component="image1"/>
        <media id='image1' src='media/AllTheStarsShownToYou.jpg' descriptor='ImageDes'/>
        <media id='image2' src='media/AuroraBorealis.jpg' descriptor='ImageDes'/>
        <media id='image3' src='media/AuroraBorealisStars.jpg' descriptor='ImageDes'/>
        <media id='image4' src='media/BeADreamer.jpg' descriptor='ImageDes'/>
        <media id='image5' src='media/DevushkaZakatPeyzazh_Pole.jpg' descriptor='ImageDes'/>
        <media id='image6' src='media/Freedom.jpg' descriptor='ImageDes'/>
        <media id='image7' src='media/ItsTooLate.jpg' descriptor='ImageDes'/>
        <media id='image8' src='media/LateNight@Mountains.jpg' descriptor='ImageDes'/>
        <media id='image9' src='media/MorningLight.jpg' descriptor='ImageDes'/>
        <media id='image10' src='media/NorthernLights-1.jpg' descriptor='ImageDes'/>
        <media id='image11' src='media/ParadiseOutsideEarth.jpg' descriptor='ImageDes'/>
        <media id='image12' src='media/PYEUy5n.jpg' descriptor='ImageDes'/>
        <media id='image13' src='media/StarsOverTheRoad.jpg' descriptor='ImageDes'/>
        <media id='image14' src='media/Sunshine@North.jpg' descriptor='ImageDes'/>
        <media id='image15' src='media/Tanity.jpg' descriptor='ImageDes'/>
        <media id='image16' src='media/Teddy-Kelley-.jpg' descriptor='ImageDes'/>
        <media id='image17' src='media/TheCalling.jpg' descriptor='ImageDes'/>
        <media id='image18' src='media/Wallhaven-415803.jpg' descriptor='ImageDes'/>
        <media id='image19' src='media/Wallhaven-527359.jpg' descriptor='ImageDes'/>
        <media id='image20' src='media/WinterSun.jpg' descriptor='ImageDes'/>
        <media id='image21' src='media/YejarSF.jpg' descriptor='ImageDes'/>
        <link id='lMoveForward1' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image1'/>
            <bind role='start' component='image2'/>
        </link>
        <link id='lMoveForward2' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image2'/>
            <bind role='start' component='image3'/>
        </link>
        <link id='lMoveForward3' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image3'/>
            <bind role='start' component='image4'/>
        </link>
        <link id='lMoveForward4' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image4'/>
            <bind role='start' component='image5'/>
        </link>
        <link id='lMoveForward5' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image5'/>
            <bind role='start' component='image6'/>
        </link>
        <link id='lMoveForward6' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image6'/>
            <bind role='start' component='image7'/>
        </link>
        <link id='lMoveForward7' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image7'/>
            <bind role='start' component='image8'/>
        </link>
        <link id='lMoveForward8' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image8'/>
            <bind role='start' component='image9'/>
        </link>
        <link id='lMoveForward9' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image9'/>
            <bind role='start' component='image10'/>
        </link>
        <link id='lMoveForward10' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image10'/>
            <bind role='start' component='image11'/>
        </link>
        <link id='lMoveForward11' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image11'/>
            <bind role='start' component='image12'/>
        </link>
        <link id='lMoveForward12' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image12'/>
            <bind role='start' component='image13'/>
        </link>
        <link id='lMoveForward13' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image13'/>
            <bind role='start' component='image14'/>
        </link>
        <link id='lMoveForward14' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image14'/>
            <bind role='start' component='image15'/>
        </link>
        <link id='lMoveForward15' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image15'/>
            <bind role='start' component='image16'/>
        </link>
        <link id='lMoveForward16' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image16'/>
            <bind role='start' component='image17'/>
        </link>
        <link id='lMoveForward17' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image17'/>
            <bind role='start' component='image18'/>
        </link>
        <link id='lMoveForward18' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image18'/>
            <bind role='start' component='image19'/>
        </link>
        <link id='lMoveForward19' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image19'/>
            <bind role='start' component='image20'/>
        </link>
        <link id='lMoveForward20' xconnector='conBase#onEndStart'>
            <bind role='onEnd' component='image20'/>
            <bind role='start' component='image21'/>
        </link>
    </body>
</ncl>
