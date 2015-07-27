<?

/*
 * GameObjectOwnershipModel class file
 *
 * @author Thomas Steinke
 */
namespace GameObject\Model;

class GameObjectOwnershipModel extends \Data\Model
{
	public $object_id;
	public $subject_id;

   public static $tableName = "object_ownership";
	public static $columns = [
		"object_id" => "string",
		"subject_id" => "string",
	];

   public static $const_columns = ["subject_id"];
}
