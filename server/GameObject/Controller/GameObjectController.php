<?

/*
 * GameObjectController class file
 *
 * @author Thomas Steinke
 */
namespace GameObject\Controller;

use \GameObject\Model\GameObjectModel;
use \Action\Controller\ActionController;

class GameObjectController
{
	/**
	 * @params two GameObjectModels
	 * 		  JSON action
	 */
	public static function act($actor, $victim, $action) {
		$action_controller = new ActionController($actor, $victim);
	
		return $action_controller->act($action);
	}
}
