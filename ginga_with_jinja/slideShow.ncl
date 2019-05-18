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
        <media id="image1" src="media/WinterSun.jpg" descriptor="ImageDes"/>
        <media id="image2" src="media/Tanity.jpg" descriptor="ImageDes"/>
        <link id="lMoveForward" xconnector="conBase#onBeginStart_delay">
            <bind role="onBegin" component="image1"/>
            <bind role="start" component="image2">
                <bindParam name="delay" value="5s"/>
            </bind>
        </link>
    </body>
</ncl>