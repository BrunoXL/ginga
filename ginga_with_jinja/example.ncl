<?xml version="1.0" encoding="UTF-8"?> 
<!-- An NCL SlideShow example with embedded Jinja2 template -->
<ncl id="slideShow">
    <head>
        <connectorBase>
            <importBase documentURI="connectorBase.ncl" alias="conBase"/>
        </connectorBase>
        <regionBase>
            <region id="main" width="100%" height="100%" zIndex="1"/>
        </regionBase>
        <descriptorBase>
            <descriptor id="ImageDes" region="main"/>
        </descriptorBase>
    </head>
    <body>
        <port id="startSlideShow" component="image1"/>
        <media id="image1" src="media/Sunshine@North.jpg" descriptor="ImageDes"/>
        <media id="image2" src="media/DevushkaZakatPeyzazh_Pole.jpg" descriptor="ImageDes"/>
        <media id="image3" src="media/StarsOverTheRoad.jpg" descriptor="ImageDes"/>
        <media id="image4" src="media/PYEUy5n.jpg" descriptor="ImageDes"/>
        <media id="image5" src="media/AllTheStarsShownToYou.jpg" descriptor="ImageDes"/>
        <media id="image6" src="media/ParadiseOutsideEarth.jpg" descriptor="ImageDes"/>
        <media id="image7" src="media/TheCalling.jpg" descriptor="ImageDes"/>
        <media id="image8" src="media/Wallhaven-527359.jpg" descriptor="ImageDes"/>
        <media id="image9" src="media/AuroraBorealis.jpg" descriptor="ImageDes"/>
        <media id="image10" src="media/Tanity.jpg" descriptor="ImageDes"/>
        <media id="image11" src="media/WinterSun.jpg" descriptor="ImageDes"/>
        <media id="image12" src="media/YejarSF.jpg" descriptor="ImageDes"/>
        <media id="image13" src="media/MorningLight.jpg" descriptor="ImageDes"/>
        <media id="image14" src="media/Teddy-Kelley-.jpg" descriptor="ImageDes"/>
        <media id="image15" src="media/Wallhaven-415803.jpg" descriptor="ImageDes"/>
        <media id="image16" src="media/BeADreamer.jpg" descriptor="ImageDes"/>
        <media id="image17" src="media/ItsTooLate.jpg" descriptor="ImageDes"/>
        <media id="image18" src="media/NorthernLights-1.jpg" descriptor="ImageDes"/>
        <media id="image19" src="media/LateNight@Mountains.jpg" descriptor="ImageDes"/>
        <media id="image20" src="media/AuroraBorealisStars.jpg" descriptor="ImageDes"/>
        <media id="image21" src="media/Freedom.jpg" descriptor="ImageDes"/>
        <link id="lMoveForward1" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image1"/>
            <bind role="start" component="image2">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward2" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image2"/>
            <bind role="start" component="image3">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward3" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image3"/>
            <bind role="start" component="image4">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward4" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image4"/>
            <bind role="start" component="image5">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward5" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image5"/>
            <bind role="start" component="image6">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward6" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image6"/>
            <bind role="start" component="image7">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward7" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image7"/>
            <bind role="start" component="image8">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward8" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image8"/>
            <bind role="start" component="image9">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward9" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image9"/>
            <bind role="start" component="image10">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward10" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image10"/>
            <bind role="start" component="image11">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward11" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image11"/>
            <bind role="start" component="image12">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward12" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image12"/>
            <bind role="start" component="image13">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward13" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image13"/>
            <bind role="start" component="image14">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward14" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image14"/>
            <bind role="start" component="image15">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward15" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image15"/>
            <bind role="start" component="image16">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward16" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image16"/>
            <bind role="start" component="image17">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward17" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image17"/>
            <bind role="start" component="image18">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward18" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image18"/>
            <bind role="start" component="image19">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward19" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image19"/>
            <bind role="start" component="image20">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
        <link id="lMoveForward20" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image20"/>
            <bind role="start" component="image21">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
    </body>
</ncl>