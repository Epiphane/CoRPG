<?

/*
 * GameObject class file
 *
 * @author Thomas Steinke
 */
namespace GameObject\Model;

require_once(__DIR__ . "/GameObjectPropertyModel.php");
require_once(__DIR__ . "/GameObjectOwnershipModel.php");

use \Data\DAO;

class GameObjectModel extends \Data\Model
{
	public static $tableName = "object";

	public static $columns = [
		"object_id" => "string",
		"game" => "string",
		"name" => "string",
		"class" => "string",
		"nickname" => "string",
		"region" => "string",
		"level" => "int",
		"experience" => "int",
		"health" => "int",
		"max_health" => "int"
	];

	public static $const_columns = [
		"object_id", "game", "name", "region"
	];

	public $object_id;
	public $game = "weebly";
	public $name;
	public $class;
	public $nickname;
	public $region;
	public $level;
	public $experience;
	public $health;
	public $max_health;
	public $properties = array();
	public $ownerships = array();

	private function setProperties($model, $properties, $existing = array()) {
		foreach ($properties as $prop => $val) {
			switch($prop) {
			case "class":
				$model->class = $val;
				break;
			case "health":
				$model->health = $val;
				break;
			case "max_health":
			case "maxhealth":
				$model->max_health = $val;
				break;
			case "level":
				$model->level = $val;
				break;
			case "experience":
				$model->experience = $val;
				break;
			default:
				if ($existing[$prop]) {
					break;
				}

				$existing[$prop] = GameObjectPropertyModel::build([
					"object_id" => $model->object_id,
					"property" => $prop,
					"value" => $val
				]);
				$existing[$prop]->save();
				break;
			}
		}

		return $existing;
	}

	public static function build($assoc) {
		$model = parent::build($assoc);

		// Search for existing properties
		$request = new \Data\Request();
		$request->Filter[] = new \Data\Filter("object_id", $model->object_id);
		
		$props = GameObjectPropertyModel::find($request);
		$properties = array();
		foreach ($props as $property) {
			$properties[$property->property] = $property;
		}

		$model->properties = self::setProperties($model, $model->properties, $properties);

		// Search for existing ownerships
		$model->ownerships = GameObjectOwnershipModel::find($request);

		return $model;
	}

	public function createPrimaryKey() {
		return md5($this->game . ":" . $this->name . ":" . $this->region);
	}

	public static function findByNameRegionGame($name, $regions = array(), $game = null) {
		$request = new \Data\Request();

		for ($i = 0; $i < count($regions); $i ++) {
			$regions[$i] = str_replace("%20", " ", $regions[$i]);
		}

		$name = str_replace("%20", " ", $name);

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
		$properties["class"] = $this->class;
		$properties["nickname"] = $this->nickname;
		$properties["experience"] = $this->experience;
		$properties["health"] = $this->health;
		$properties["max_health"] = $this->max_health;

		$possessions = array();
		foreach($this->ownerships as $obj) {
			$possessions[] = $obj->subject_id;
		}

		return [
			"object_id" => $this->object_id,
			"name" => $this->name,
			"region" => $this->region,
			"properties" => $properties,
			"possessions" => $possessions
		];
	}

	public function update($attrs) {
		$myProps = array();

		if ($attrs["name"]) {
			if ($attrs["name"] !== $this->name) {
				$attrs["nickname"] = $attrs["name"];
			}
		}

		// Don't update restricted stuff
		foreach (self::$const_columns as $restricted) {
			unset($attrs[$restricted]);
		}

		// Move properties to where they belong
		$properties = $attrs["properties"] ?: array();
		foreach ($properties as $prop => $val) {
			if (self::$columns[$prop]) {
				$myProps[$prop] = $val;
			}
			else {
				if ($this->properties[$prop]) {
					$result = $this->properties[$prop]->update([
						"value" => $val
					]);
				}
				else {
					$this->properties[$prop] = $property = \GameObject\Model\GameObjectPropertyModel::build([
						"object_id" => $this->object_id,
						"property" => $prop,
						"value" => $val
					]);
		
					$result = $property->save();
				}
		
				if (!$result)
					throw new \Exception("Property " . $prop . " failed to update");
			}
		}

		unset($attrs["properties"]);
		foreach ($attrs as $prop => $val) {
			if (self::$columns[$prop]) {
				$myProps[$prop] = $val;
			}
			else {
				throw new \Exception("Property " . $prop . " is not part of GameObjectModel. Please add to 'properties'");
			}
		}

		if (count($myProps) > 0) {
			return parent::update($myProps);
		}

		return $this;
	}
}
