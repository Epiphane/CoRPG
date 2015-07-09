<?

/*
 * GameObject class file
 *
 * @author Thomas Steinke
 */
namespace GameObject\Model;

require_once(__DIR__ . "/GameObjectPropertyModel.php");

use \Data\DAO;

class GameObjectModel extends \Data\Model
{
	public static $tableName = "object";

	public static $columns = [
		"object_id" => "string",
		"name" => "string",
		"region" => "string",
		"level" => "int",
		"health" => "int",
		"max_health" => "int"
	];

	public static $const_columns = [
		"object_id", "name", "region"
	];

	public $object_id;
	public $name;
	public $region;
	public $level;
	public $health;
	public $max_health;
	public $properties;

	public static function findByNameRegionGame($name, $regions = array(), $game = null) {
		$request = new \Data\Request();

		$request->Filter[] = new \Data\Filter("name", $name);
		if (is_array($regions) && count($regions) > 0)
			$request->Filter[] = new \Data\InFilter("region", $regions);
		if ($game)
			$request->Filter[] = new \Data\Filter("game", $game);

		return self::findOne($request);
	}

	public function read() {
		$properties = array();
		foreach($this->properties as $prop) {
			$properties[$prop->property] = $prop->value;
		}

		$properties["level"] = $this->level;
		$properties["health"] = $this->health;
		$properties["max_health"] = $this->max_health;

		return [
			"name" => $this->name,
			"region" => $this->region,
			"properties" => $properties
		];
	}

	public static function build($assoc) {
		$model = parent::build($assoc);

		$request = new \Data\Request();
		$request->Filter[] = new \Data\Filter("object_id", $model->object_id);

		$model->properties = GameObjectPropertyModel::find($request);

		return $model;
	}
}
