import gfx.core.UIComponent;
import gfx.events.EventTypes;
import gfx.io.GameDelegate;

class KeyValueSet extends UIComponent {

	public var key: TextField;
	public var value: TextField;

	/* PRIVATE VARIABLES */
	private var _dataProvider: Object;


	/* INITIALIZATION */
	public function KeyValueSet() {
		super();
        log("Running KeyValueSet ...");

		//var _loader = new MovieClipLoader();
		//_loader.addListener(this);
		//_loader.loadClip("avatars_for_dmni.swf", textIcon);
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
		log("key value: " + keyText + ", value value: " + valueText);

		if (keyText == null) {
			return;
		}

		if (valueText == null) {
			return;
		}

		key.autoSize = "left";
		value.autoSize = "left";

		key.text = keyText;
		key._visible = true;

		value.text = valueText;
		value._visible = true;

	}

	/* PRIVATE FUNCTIONS */
	
	private function configUI(): Void {
		super.configUI();
	}


	private function log(a_message: String): Void {
		GameDelegate.call("Log", [a_message]);
	}
}