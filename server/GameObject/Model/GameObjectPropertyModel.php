<?

/*
 * GameObjectProperty class file
 *
 * @author Thomas Steinke
 */
namespace GameObject\Model;

class GameObjectPropertyModel extends \Data\Model
{
	public $object_id;
	public $property;
	public $value;

	public static $tableName = "object_properties";
	public static $columns = [
		"object_id" => "string",
		"property" => "string",
		"value" => "string",
	];
}
