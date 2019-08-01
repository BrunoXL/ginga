<?xml version="1.0" encoding="UTF-8"?> 
<!--NCL configure file to run templates on client side-->
<ncl>
    <head>
        <connectorBase>
            <importBase documentURI="connectorBase.ncl" alias="conBase"/>
        </connectorBase>
        <regionBase>
            <region id="main" width="100%" height="100%" zIndex="1"/>
        </regionBase>
        <descriptorBase>
            <descriptor id="LuaDes" region="main" explicitDur="1s"/>
        </descriptorBase>
    </head>
    <body>
        <port id="template-handler" component="template_processor"/>
        <media id="final_ncl" src="slideShow.ncl"/>
        <media id="template_processor" src="template_processor.lua">
            <property name="engine" value="mustache"/>
            <property name="template" value="slideShow.ncl.mustache"/>
            <property name="partial1" value="medias.ncl.j2"/>
            <property name="partial2" value="links.ncl.j2"/>
            <property name="padding" value="padding.json"/>
        </media> 
        <link id="link" xconnector="conBase#onEndStart">
            <bind role="onEnd" component="template_processor"/>
            <bind role="start" component="final_ncl"/>
        </link> 
    </body>
</ncl>