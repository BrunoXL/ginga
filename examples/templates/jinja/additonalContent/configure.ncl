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
        <media id="final_ncl" src="additionalContent.ncl"/>
        <media id="template_processor" src="template_processor.lua">
            <property name="engine" value="jinja" />
            <property name="template" value="addtionalContent_child.ncl.j2"/>
            <property name="padding" value="padding.json"/>
        </media> 
        <link id="link" xconnector="conBase#onEndStart">
            <bind role="onEnd" component="template_processor"/>
            <bind role="start" component="final_ncl"/>
        </link> 
    </body>
</ncl>