import gfx.core.UIComponent;
import gfx.events.EventTypes;
import gfx.io.GameDelegate;

class KeyValueSetSpecial extends UIComponent {

	public var key: TextField;
	public var value: TextField;
	public var valueMax: TextField;

	/* PRIVATE VARIABLES */
	private var _dataProvider: Object;

	private var _valueText: String;
	private var _keyText: String;
	private var _valueMaxText: String;
	
	/* INITIALIZATION */
	public function KeyValueSetSpecial() {
		super();
        //log("Running KeyValueSetSpecial ...");
	}
	
	public function onLoad(): Void {
        
	}

	public function get dataProvider(): Object
	{
		return _dataProvider;
	}

	public function set dataProvider(dataValue: Object): Void
	{
		if (_dataProvider == dataValue) {
			return;
		}

		_dataProvider = dataValue;

		if (_dataProvider == null) {
			return;
		}

		var keyText: String = _dataProvider.keyText != null ? _dataProvider.keyText : "";
		var valueText: String = _dataProvider.valueText != null ? _dataProvider.valueText : "";
		var valueMaxText: String = _dataProvider.valueMaxText != null ? _dataProvider.valueMaxText : "";

		if (valueText == _valueText) {
			return;
		}

		log("key value: " + keyText + ", value value: " + valueText + ", max value: " + valueMaxText);

		if (keyText == null) {
			return;
		}

		if (valueText == null) {
			return;
		}
		
		if (valueMaxText == null) {
			return;
		}

		if (_keyText != keyText ) {
			key.autoSize = "left";
			key.text = keyText;
			key._visible = true;
			_keyText = keyText;
		}

		if (_valueText != valueText) {
			value.autoSize = "left";
			value.text = valueText;
			value._visible = true;
			_valueText = valueText;
		}
		
		if (_valueMaxText != valueMaxText) {
			valueMax.autoSize = "left";
			valueMax.text = valueMaxText;
			valueMax._visible = true;
			_valueMaxText = valueMaxText;
		}

		log("set all data.");
	}

	/* PRIVATE FUNCTIONS */
	
	private function configUI(): Void {
		super.configUI();
	}


	private function log(a_message: String): Void {
		GameDelegate.call("Log", [a_message]);
	}
}