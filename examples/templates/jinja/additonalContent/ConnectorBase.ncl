<?xml version="1.0" encoding="ISO-8859-1"?>
<ncl id="composerConnBase" xmlns="http://www.ncl.org.br/NCL3.0/CausalConnectorProfile" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.ncl.org.br/NCL3.0/CausalConnectorProfile http://www.ncl.org.br/NCL3.0/profiles/NCL30CausalConnector.xsd">
	<head>
		<connectorBase>
			<causalConnector id="onBeginStart">
				<simpleCondition role="onBegin"/>
				<simpleAction role="start"/>
			</causalConnector>
			
			<causalConnector id="onBeginStop">
				<simpleCondition role="onBegin"/>
				<simpleAction role="stop"/>
			</causalConnector>

			<causalConnector id="onBeginAbort">
				<simpleCondition role="onBegin"/>
				<simpleAction role="abort"/>
			</causalConnector>
			
			<causalConnector id="onBeginPause">
				<simpleCondition role="onBegin"/>
				<simpleAction role="pause"/>
			</causalConnector>
			
			<causalConnector id="onBeginResume">
				<simpleCondition role="onBegin"/>
				<simpleAction role="resume"/>
			</causalConnector>
			
			<causalConnector id="onBeginSet">
				<connectorParam name="var"/>
				<simpleCondition role="onBegin"/>
				<simpleAction role="set" value="$var"/>
			</causalConnector>
			
			<causalConnector id="onEndStart">
				<simpleCondition role="onEnd"/>
				<simpleAction role="start"/>
			</causalConnector>
			
			<causalConnector id="onEndStop">
				<simpleCondition role="onEnd"/>
				<simpleAction role="stop"/>
			</causalConnector>
			
			<causalConnector id="onEndAbort">
				<simpleCondition role="onEnd"/>
				<simpleAction role="abort"/>
			</causalConnector>
			
			<causalConnector id="onEndPause">
				<simpleCondition role="onEnd"/>
				<simpleAction role="pause"/>
			</causalConnector>
			
			<causalConnector id="onEndResume">
				<simpleCondition role="onEnd"/>
				<simpleAction role="resume"/>
			</causalConnector>

			<causalConnector id="onEndSet">
				<connectorParam name="var"/>
				<simpleCondition role="onEnd"/>
				<simpleAction role="set" value="$var"/>
			</causalConnector>
			
			<causalConnector id="onSelectionStart">
				<simpleCondition role="onSelection"/>
				<simpleAction role="start"/>
			</causalConnector>

			<causalConnector id="onSelectionStop">
				<simpleCondition role="onSelection"/>
				<simpleAction role="stop"/>
			</causalConnector>

			<causalConnector id="onSelectionAbort">
				<simpleCondition role="onSelection"/>
				<simpleAction role="abort"/>
			</causalConnector>

			<causalConnector id="onSelectionPause">
				<simpleCondition role="onSelection"/>
				<simpleAction role="pause"/>
			</causalConnector>

			<causalConnector id="onSelectionResume">
				<simpleCondition role="onSelection"/>
				<simpleAction role="resume"/>
			</causalConnector>
			
			<causalConnector id="onSelectionSet">
				<connectorParam name="var"/>
				<simpleCondition role="onSelection"/>
				<simpleAction role="set" value="$var"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStart">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="start"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStop">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="stop"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionAbort">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="abort"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionPause">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="pause"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionResume">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="resume"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionSet">
				<connectorParam name="keyCode"/>
				<connectorParam name="var"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="set" value="$var"/>
			</causalConnector>
			
			<causalConnector id="onBeginStartN">
				<simpleCondition role="onBegin"/>
				<simpleAction role="start" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onBeginStopN">
				<simpleCondition role="onBegin"/>
				<simpleAction role="stop" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onBeginAbortN">
				<simpleCondition role="onBegin"/>
				<simpleAction role="abort" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onBeginPauseN">
				<simpleCondition role="onBegin"/>
				<simpleAction role="pause" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onBeginResumeN">
				<simpleCondition role="onBegin"/>
				<simpleAction role="resume" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onBeginSetN">
				<connectorParam name="var"/>
				<simpleCondition role="onBegin"/>
				<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onBeginStartNStopN">
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>

			<causalConnector id="onBeginStartNPauseN">
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="pause" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginStartNResumeN">
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="resume" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginStartNSetN">
				<connectorParam name="var"/>
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginStartNDelayStopN">
				<connectorParam name="delay"/>
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginStopNStartN">
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="start" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginStopNPauseN">
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="pause" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginStopNResumeN">
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="resume" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginStopNSetN">
				<connectorParam name="var"/>
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginStopNDelayStartN">
				<connectorParam name="delay"/>
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="start" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginSetVarNDelayStopN">
				<connectorParam name="delay"/>
				<connectorParam name="var"/>
				<simpleCondition role="onBegin"/>
				<compoundAction operator="seq">
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStartN">
				<simpleCondition role="onEnd"/>
				<simpleAction role="start" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onEndStopN">
				<simpleCondition role="onEnd"/>
				<simpleAction role="stop" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onEndAbortN">
				<simpleCondition role="onEnd"/>
				<simpleAction role="abort" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onEndPauseN">
				<simpleCondition role="onEnd"/>
				<simpleAction role="pause" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onEndResumeN">
				<simpleCondition role="onEnd"/>
				<simpleAction role="resume" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onEndSetN">
				<connectorParam name="var"/>
				<simpleCondition role="onEnd"/>
				<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onEndStartNStopN">
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStartNPauseN">
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="pause" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStartNResumeN">
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="resume" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStartNSetN">
				<connectorParam name="var"/>
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStartNDelayStopN">
				<connectorParam name="delay"/>
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStopNStartN">
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="start" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStopNPauseN">
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="pause" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStopNResumeN">
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="resume" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStopNSetN">
				<connectorParam name="var"/>
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndStopNDelayStartN">
				<connectorParam name="delay"/>
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="start" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndSetVarNDelayStopN">
				<connectorParam name="delay"/>
				<connectorParam name="var"/>
				<simpleCondition role="onEnd"/>
				<compoundAction operator="seq">
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStartN">
				<simpleCondition role="onSelection"/>
				<simpleAction role="start" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onSelectionStopN">
				<simpleCondition role="onSelection"/>
				<simpleAction role="stop" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onSelectionAbortN">
				<simpleCondition role="onSelection"/>
				<simpleAction role="abort" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onSelectionPauseN">
				<simpleCondition role="onSelection"/>
				<simpleAction role="pause" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onSelectionResumeN">
				<simpleCondition role="onSelection"/>
				<simpleAction role="resume" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onSelectionSetN">
				<connectorParam name="var"/>
				<simpleCondition role="onSelection"/>
				<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onSelectionStartNStopN">
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStartNPauseN">
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="pause" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStartNResumeN">
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="resume" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStartNSetN">
				<connectorParam name="var"/>
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStartNDelayStopN">
				<connectorParam name="delay"/>
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStopNStartN">
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="start" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStopNPauseN">
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="pause" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStopNResumeN">
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="resume" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStopNSetN">
				<connectorParam name="var"/>
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onSelectionStopNDelayStartN">
				<connectorParam name="delay"/>
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="start" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStartNStopNSet">
				<connectorParam name="keyCode"/>
				<connectorParam name="var"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="set" value="$var"/>
				</compoundAction>
				</causalConnector>
			<causalConnector id="onSelectionSetVarNDelayStopN">
				<connectorParam name="delay"/>
				<connectorParam name="var"/>
				<simpleCondition role="onSelection"/>
				<compoundAction operator="seq">
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStartN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="start" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStopN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="stop" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionAbortN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="abort" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionPauseN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="pause" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionResumeN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="resume" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionSetN">
				<connectorParam name="keyCode"/>
				<connectorParam name="var"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStartNStopN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStartNPauseN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="pause" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStartNResumeN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="resume" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStartNSetN">
				<connectorParam name="keyCode"/>
				<connectorParam name="var"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStartNDelayStopN">
				<connectorParam name="keyCode"/>
				<connectorParam name="delay"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStopNStartN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="start" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStopNPauseN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="pause" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStopNResumeN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="resume" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStopNSetN">
				<connectorParam name="keyCode"/>
				<connectorParam name="var"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="par">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionStopNDelayStartN">
				<connectorParam name="keyCode"/>
				<connectorParam name="delay"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="start" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionSetVarNDelayStopN">
				<connectorParam name="keyCode"/>
				<connectorParam name="delay"/>
				<connectorParam name="var"/>
				<simpleCondition role="onSelection" key="$keyCode"/>
				<compoundAction operator="seq">
					<simpleAction role="set" value="$var" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" delay="$delay" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNSet">
				<connectorParam name="keyCode"/>
				<connectorParam name="var"/>
				<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
				<simpleAction role="set" value="$var"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNStartN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
				<simpleAction role="start" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNStopN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
				<simpleAction role="stop" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNAbortN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
				<simpleAction role="abort" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNPauseN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
				<simpleAction role="pause" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNResumeN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
				<simpleAction role="start" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNStartNStopN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNStopNStartN">
				<connectorParam name="keyCode"/>
				<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
				<compoundAction operator="seq">
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
					<simpleAction role="start" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginAttNodeTestStartN">
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onBegin"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<simpleAction role="start" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onBeginAttNodeTestStopN">
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onBegin"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<simpleAction role="stop" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onBeginAttNodeTestAbortN">
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onBegin"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<simpleAction role="abort" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onEndAttNodeTestStartN">
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onEnd"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<simpleAction role="start" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onEndAttNodeTestStopN">
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onEnd"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<simpleAction role="stop" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onEndAttNodeTestAbortN">
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onEnd"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<simpleAction role="abort" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNAttNodeTestStartN">
				<connectorParam name="keyCode"/>
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<simpleAction role="start" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onKeySelectionNAttNodeTestStopN">
				<connectorParam name="keyCode"/>
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onSelection" key="$keyCode" max="unbounded" qualifier="or"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<simpleAction role="stop" max="unbounded" qualifier="par"/>
			</causalConnector>
			
			<causalConnector id="onBeginAttNodeTestStartNStopN">
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onBegin"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndAttNodeTestStartNStopN">
				<connectorParam name="value"/>
				<compoundCondition operator="and">
					<simpleCondition role="onEnd"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<compoundAction operator="seq">
					<simpleAction role="start" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onBeginAttNodeTestSetNStartN">
				<connectorParam name="value"/>
				<connectorParam name="setValue"/>
				<compoundCondition operator="and">
					<simpleCondition role="onBegin"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<compoundAction operator="seq">
					<simpleAction role="set" value="$setValue" max="unbounded" qualifier="par"/>
					<simpleAction role="start" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
			
			<causalConnector id="onEndAttNodeTestSetNStopN">
				<connectorParam name="value"/>
				<connectorParam name="setValue"/>
				<compoundCondition operator="and">
					<simpleCondition role="onEnd"/>
					<assessmentStatement comparator="eq">
						<attributeAssessment role="attNodeTest" eventType="attribution" attributeType="nodeProperty"/>
						<valueAssessment value="$value"/>
					</assessmentStatement>
				</compoundCondition>
				<compoundAction operator="seq">
					<simpleAction role="set" value="$setValue" max="unbounded" qualifier="par"/>
					<simpleAction role="stop" max="unbounded" qualifier="par"/>
				</compoundAction>
			</causalConnector>
		</connectorBase>
	</head>
</ncl>