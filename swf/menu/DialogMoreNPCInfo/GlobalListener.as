import gfx.events.EventTypes;
import gfx.io.GameDelegate;

class GlobalListener extends MovieClip {
	/* PRIVATE VARIABLES */
	private var _close: Button;


	/* INITIALIZATION */
	public function GlobalListener() {
		super();
        log("Running Listener ...");
	}


	public function onLoad(): Void {
        
	}


	/* PRIVATE FUNCTIONS */

	private function log(a_message: String): Void {
		GameDelegate.call("Log", [a_message]);
	}
}
