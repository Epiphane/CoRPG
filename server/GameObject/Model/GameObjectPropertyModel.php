<?

/*
 * GameObjectProperty class file
 *
 * @author Thomas Steinke
 */
namespace GameObject\Model;

class GameObjectPropertyModel extends \Data\Model
{
	public $property_id;
	public $object_id;
	public $property;
	public $value;

	public static $tableName = "object_properties";
	public static $columns = [
		"property_id" => "int",
		"object_id" => "string",
		"property" => "string",
		"value" => "string",
	];

	public static $const_columns = ["property_id"];
}
