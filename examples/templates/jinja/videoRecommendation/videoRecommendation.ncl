<?xml version="1.0" encoding="UTF-8"?> 
<!--An NCL video Recommendation example with embedded Jinja2 template-->
<ncl>
    <head>
        <connectorBase>
            <importBase documentURI="connectorBase.ncl" alias="conBase"/>
        </connectorBase>
        <ruleBase>
            <rule id="ruleFocus1" var="service.currentFocus" value="1" comparator="eq"/>
            <rule id="ruleFocus2" var="service.currentFocus" value="2" comparator="eq"/>
            <rule id="ruleFocus3" var="service.currentFocus" value="3" comparator="eq"/>
        </ruleBase> 
        <regionBase>
            <region id="rKeepAlive" width="100%" height="100%" zIndex="1"/>
            <region id="rMain" width="100%" height="100%" zIndex="2"/>
            <region id="rLeft" top="80%" left="10%" width="20%" height="15%" zIndex="3"/>
            <region id="rCenter" top="80%" left="40%" width="20%" height="15%" zIndex="3"/>
            <region id="rRight" top="80%" left="70%" width="20%" height="15%" zIndex="3"/>
        </regionBase>
        <descriptorBase>
            <descriptor id="desMainVideo" region="rMain"/>
            <descriptor id="desKeepAlive" region="rKeepAlive"/>
            <descriptor id="desRegion1" region="rLeft"
                focusIndex="1" moveRight="2" />
            <descriptor id="desRegion2" region="rCenter"
                focusIndex="2" moveLeft="1" moveRight="3"/>
            <descriptor id="desRegion3" region="rRight"
                focusIndex="3" moveLeft="2"/>
        </descriptorBase>
    </head>
    <body>
        <port id="start" component="mMainVideo"/>
        <port id="start2" component="mKeepAlive"/>

        <media id="mMainVideo" src="media/mainVideo.mp4" descriptor="desMainVideo"/>
        <media id="mNodeSettings" type="application/x-ginga-settings">
            <property name="service.currentFocus"/>
            <property name="selected" value="false"/>
            <property name="direction" value="right"/>
        </media>

        <media id="mKeepAlive" src="media/background.jpg" descriptor="desKeepAlive" />

        <context id="ctx123">
            <port id="startmLeft123" component="mLeft123"/> 
            <port id="startmCenter123" component="mCenter123"/> 
            <port id="startmRight123" component="mRight123"/> 

            <media id="mLeft123" src="media/image1.jpg" descriptor="desRegion1"/>
            <media id="mCenter123" src="media/image2.jpg" descriptor="desRegion2"/>
            <media id="mRight123" src="media/image3.jpg" descriptor="desRegion3"/>
            <media id="mMainVideo123" refer="mMainVideo" instance="instSame"/> 
            <media id="mNodeSettings123" type="application/x-ginga-settings"> 
                <property name="service.currentFocus"/>
                <property name="selected"/>
                <property name="direction"/>
            </media>
            <switch id="videos123">        
                            <bindRule constituent="selectionVideo1_1" rule="ruleFocus1"/>
                <bindRule constituent="selectionVideo2_2" rule="ruleFocus2"/>
                <bindRule constituent="selectionVideo3_3" rule="ruleFocus3"/>
                <media id="selectionVideo1_1" src="media/video1.mp4" descriptor="desMainVideo"/>
                <media id="selectionVideo2_2" src="media/video2.mp4" descriptor="desMainVideo"/>
                <media id="selectionVideo3_3" src="media/video3.mp4" descriptor="desMainVideo"/>
            </switch>

            <link id="lPickupVideo123" xconnector="conBase#onKeySelectionStopNStartNSet">
                <bind role="onSelection" component="mLeft123"/>
            <bind role="onSelection" component="mCenter123"/>
            <bind role="onSelection" component="mRight123"/>
                <bind role="stop" component="mLeft123"/>
            <bind role="stop" component="mCenter123"/>
            <bind role="stop" component="mRight123"/>
            <bind role="stop" component="mMainVideo123"/>
            <bind role="start" component="videos123"/>
            <bind role="set" component="mNodeSettings123" interface="selected">
                <bindParam name="var" value="true"/>
             </bind>
            </link>

            <link id="lSetCtx234Right" xconnector="conBase#onKeySelectionTestStopNSetN">
            <linkParam name="keyCode" value="CURSOR_RIGHT"/>
                <bind role="shift" component="mNodeSettings123" interface="service.currentFocus">
                <bindParam name="focusValue" value="3"/>
             </bind>
            <bind role="onSelection" component="mRight123"/>
                <bind role="stop" component="ctx123"/>
            <bind role="set" component="mNodeSettings123" interface="direction">
                <bindParam name="setValue" value="right"/>
             </bind>
            </link>
        </context>

        <context id="ctx234">
            <port id="startmLeft234" component="mLeft234"/> 
            <port id="startmCenter234" component="mCenter234"/> 
            <port id="startmRight234" component="mRight234"/> 

            <media id="mLeft234" src="media/image2.jpg" descriptor="desRegion1"/>
            <media id="mCenter234" src="media/image3.jpg" descriptor="desRegion2"/>
            <media id="mRight234" src="media/image4.jpg" descriptor="desRegion3"/>
            <media id="mMainVideo234" refer="mMainVideo" instance="instSame"/> 
            <media id="mNodeSettings234" type="application/x-ginga-settings"> 
                <property name="service.currentFocus"/>
                <property name="selected"/>
                <property name="direction"/>
            </media>
            <switch id="videos234">        
                            <bindRule constituent="selectionVideo2_1" rule="ruleFocus1"/>
                <bindRule constituent="selectionVideo3_2" rule="ruleFocus2"/>
                <bindRule constituent="selectionVideo4_3" rule="ruleFocus3"/>
                <media id="selectionVideo2_1" src="media/video2.mp4" descriptor="desMainVideo"/>
                <media id="selectionVideo3_2" src="media/video3.mp4" descriptor="desMainVideo"/>
                <media id="selectionVideo4_3" src="media/video4.mp4" descriptor="desMainVideo"/>
            </switch>

            <link id="lPickupVideo234" xconnector="conBase#onKeySelectionStopNStartNSet">
                <bind role="onSelection" component="mLeft234"/>
            <bind role="onSelection" component="mCenter234"/>
            <bind role="onSelection" component="mRight234"/>
                <bind role="stop" component="mLeft234"/>
            <bind role="stop" component="mCenter234"/>
            <bind role="stop" component="mRight234"/>
            <bind role="stop" component="mMainVideo234"/>
            <bind role="start" component="videos234"/>
            <bind role="set" component="mNodeSettings234" interface="selected">
                <bindParam name="var" value="true"/>
             </bind>
            </link>

            <link id="lSetCtx345Right" xconnector="conBase#onKeySelectionTestStopNSetN">
            <linkParam name="keyCode" value="CURSOR_RIGHT"/>
                <bind role="shift" component="mNodeSettings234" interface="service.currentFocus">
                <bindParam name="focusValue" value="3"/>
             </bind>
            <bind role="onSelection" component="mRight234"/>
                <bind role="stop" component="ctx234"/>
            <bind role="set" component="mNodeSettings234" interface="direction">
                <bindParam name="setValue" value="right"/>
             </bind>
            </link>

            <link id="lSetCtx123Left" xconnector="conBase#onKeySelectionTestStopNSetN">
            <linkParam name="keyCode" value="CURSOR_LEFT"/>
                <bind role="shift" component="mNodeSettings234" interface="service.currentFocus">
                <bindParam name="focusValue" value="1"/>
             </bind>
            <bind role="onSelection" component="mLeft234"/>
            <bind role="set" component="mNodeSettings234" interface="service.currentFocus">
                <bindParam name="setValue" value="3"/>
            </bind>
            <bind role="set" component="mNodeSettings234" interface="direction">
                <bindParam name="setValue" value="left"/>
            </bind>
            <bind role="stop" component="ctx234"/>
            </link>
        </context>

        <context id="ctx345">
            <port id="startmLeft345" component="mLeft345"/> 
            <port id="startmCenter345" component="mCenter345"/> 
            <port id="startmRight345" component="mRight345"/> 

            <media id="mLeft345" src="media/image3.jpg" descriptor="desRegion1"/>
            <media id="mCenter345" src="media/image4.jpg" descriptor="desRegion2"/>
            <media id="mRight345" src="media/image5.jpg" descriptor="desRegion3"/>
            <media id="mMainVideo345" refer="mMainVideo" instance="instSame"/> 
            <media id="mNodeSettings345" type="application/x-ginga-settings"> 
                <property name="service.currentFocus"/>
                <property name="selected"/>
                <property name="direction"/>
            </media>
            <switch id="videos345">        
                            <bindRule constituent="selectionVideo3_1" rule="ruleFocus1"/>
                <bindRule constituent="selectionVideo4_2" rule="ruleFocus2"/>
                <bindRule constituent="selectionVideo5_3" rule="ruleFocus3"/>
                <media id="selectionVideo3_1" src="media/video3.mp4" descriptor="desMainVideo"/>
                <media id="selectionVideo4_2" src="media/video4.mp4" descriptor="desMainVideo"/>
                <media id="selectionVideo5_3" src="media/video5.mp4" descriptor="desMainVideo"/>
            </switch>

            <link id="lPickupVideo345" xconnector="conBase#onKeySelectionStopNStartNSet">
                <bind role="onSelection" component="mLeft345"/>
            <bind role="onSelection" component="mCenter345"/>
            <bind role="onSelection" component="mRight345"/>
                <bind role="stop" component="mLeft345"/>
            <bind role="stop" component="mCenter345"/>
            <bind role="stop" component="mRight345"/>
            <bind role="stop" component="mMainVideo345"/>
            <bind role="start" component="videos345"/>
            <bind role="set" component="mNodeSettings345" interface="selected">
                <bindParam name="var" value="true"/>
             </bind>
            </link>

            <link id="lSetCtx234Left" xconnector="conBase#onKeySelectionTestStopNSetN">
            <linkParam name="keyCode" value="CURSOR_LEFT"/>
                <bind role="shift" component="mNodeSettings345" interface="service.currentFocus">
                <bindParam name="focusValue" value="1"/>
             </bind>
            <bind role="onSelection" component="mLeft345"/>
                <bind role="stop" component="ctx345"/>
            <bind role="set" component="mNodeSettings345" interface="service.currentFocus">
                <bindParam name="setValue" value="3"/>
             </bind>
            <bind role="set" component="mNodeSettings345" interface="direction">
                <bindParam name="setValue" value="left"/>
             </bind>
            </link>
        </context>

        <link id="lBeginCtx123" xconnector="conBase#onBeginStart">
            <bind role="onBegin" component="mMainVideo"/>
            <bind role="start" component="ctx123"/>
        </link>

            <link id="lStartCtx234Right" xconnector="conBase#onEndAttNodeTestStartN">
                <bind role="onEnd" component="ctx123"/>
            <bind role="attNodeTest2" component="mNodeSettings" interface="direction">
                <bindParam name="value2" value="right"/>
             </bind>
            <bind role="attNodeTest" component="mNodeSettings" interface="selected">
                <bindParam name="value" value="false"/>
             </bind>
                <bind role="start" component="ctx234"/>
            </link>
        
            <link id="lStartCtx345Right" xconnector="conBase#onEndAttNodeTestStartN">
                <bind role="onEnd" component="ctx234"/>
            <bind role="attNodeTest2" component="mNodeSettings" interface="direction">
                <bindParam name="value2" value="right"/>
             </bind>
            <bind role="attNodeTest" component="mNodeSettings" interface="selected">
                <bindParam name="value" value="false"/>
             </bind>
                <bind role="start" component="ctx345"/>
            </link> 

            <link id="lStartCtx123Left" xconnector="conBase#onEndAttNodeTestStartNSet">
                <bind role="onEnd" component="ctx234"/>
            <bind role="set" component="mNodeSettings" interface="service.currentFocus">
                <bindParam name="setValue" value="3"/>
             </bind>
            <bind role="attNodeTest2" component="mNodeSettings" interface="direction">
                <bindParam name="value2" value="left"/>
             </bind>
            <bind role="attNodeTest" component="mNodeSettings" interface="selected">
                <bindParam name="value" value="false"/>
             </bind>
                <bind role="start" component="ctx123"/>
            </link>

            <link id="lStartCtx234Left" xconnector="conBase#onEndAttNodeTestStartNSet">
                <bind role="onEnd" component="ctx345"/>
            <bind role="set" component="mNodeSettings" interface="service.currentFocus">
                <bindParam name="setValue" value="3"/>
             </bind>
            <bind role="attNodeTest2" component="mNodeSettings" interface="direction">
                <bindParam name="value2" value="left"/>
             </bind>
            <bind role="attNodeTest" component="mNodeSettings" interface="selected">
                <bindParam name="value" value="false"/>
             </bind>
                <bind role="start" component="ctx234"/>
            </link>
    </body>
</ncl>