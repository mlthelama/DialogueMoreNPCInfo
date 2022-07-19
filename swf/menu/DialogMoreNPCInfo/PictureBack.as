import gfx.core.UIComponent;
import gfx.events.EventTypes;
import gfx.io.GameDelegate;

class PictureBack extends UIComponent {

	public var textIcon: MovieClip;

	/* PRIVATE VARIABLES */
	private var _dataProvider: Object;


	/* INITIALIZATION */
	public function PictureBack() {
		super();
        log("Running PictureBack ...");

		var _loader = new MovieClipLoader();
		_loader.addListener(this);
		_loader.loadClip("avatars_for_dmni.swf", textIcon);
	}


	public function onLoad(): Void {
        
	}

	public function get dataProvider(): Object
	{
		return _dataProvider;
	}

	public function set dataProvider(value: Object): Void
	{
		if (_dataProvider == value) {
			return;
		}

		_dataProvider = value;

		if (_dataProvider == null) {
			return;
		}

		var key: String = _dataProvider.iconKey != null ? _dataProvider.iconKey : "";
		log("key value: " + key);

		if (key == null) {
			return;
		}

		textIcon.gotoAndStop(key);
		textIcon._width = 140;
		textIcon._height = 140;
		textIcon._visible = true;
	}

	/* PRIVATE FUNCTIONS */
	
	private function configUI(): Void {
		super.configUI();
	}


	private function log(a_message: String): Void {
		GameDelegate.call("Log", [a_message]);
	}
}