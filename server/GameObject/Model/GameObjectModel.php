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
		"experience" => "int",
		"health" => "int",
		"max_health" => "int"
	];

	public static $const_columns = [
		"object_id", "name", "region"
	];

	public $object_id;
	public $game = "weebly";
	public $name;
	public $region;
	public $level;
	public $experience;
	public $health;
	public $max_health;
	public $properties = array();

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

		foreach ($model->properties as $prop => $val) {
			switch($prop) {
			case "health":
				$model->name = $val;
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
				if ($properties[$prop]) {
					break;
				}

				$properties[$prop] = GameObjectPropertyModel::build([
					"object_id" => $model->object_id,
					"property" => $prop,
					"value" => $val
				]);
				$properties[$prop]->save();
				break;
			}
		}

		$model->properties = $properties;

		return $model;
	}

	public function createPrimaryKey() {
		return md5($this->game . ":" . $this->name . ":" . $this->region);
	}

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

	public function update($attrs) {
		$myProps = array();

		foreach ($attrs as $prop => $val) {
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

		if (count($myProps) > 0) {
			$result = parent::update($myProps);

			if (!$result)
				throw new \Exception("GameObject failed to update");
		}

		return [
			"success" => true
		];
	}
}
