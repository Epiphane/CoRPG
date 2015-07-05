<?

/*
 * Data Access Object (DAO) class file
 *
 * @author Thomas Steinke
 */
namespace Data;

require_once(__DIR__ . "/DB.php");
require_once(__DIR__ . "/Model.php");
require_once(__DIR__ . "/Request.php");
require_once(__DIR__ . "/Filter.php");
require_once(__DIR__ . "/InFilter.php");

class DAO
{
	public $connection = null;
	public $Model      = null;
	public $tableName  = null;
	public $colTypes   = [];

	public static $supported_types = [ "int" => "i", "string" => "s" ];

	public function __construct($model) {
		$this->Model     = $model;
		$this->tableName = $model::$tableName;

		foreach ($model::$columns as $col => $type) {
			$this->colTypes[$col] = self::$supported_types[$type];
		}

		$this->initDbConnection();
	}

	protected function initDbConnection() {
		$this->connection = \Data\DB::getConnection();
	}

	public function build($model, $object) {
		return $model::build($object);
	}

	public function findOne($request) {
		$result = $this->find($request);

		if (count($result) > 0) {
			return $result[0];
		}
		else {
			return null;
		}
	}

	public function find($request) {
		$query  = "SELECT * FROM " . $this->tableName;
		$values = [];
		$types  = [];
		if (count($request->Filter) > 0) {
			$qFilters = [];
			foreach ($request->Filter as $filter) {
				if ($filter instanceof InFilter) {
					$qFilters[] = $filter->property . " " . $filter->comparator . " (" . join(", ", array_fill(0, count($filter->value), "?")) . ")";
					for ($i = 0; $i < count($filter->value); $i ++) {
						$values[] = &$filter->value[$i];
						$types[] = $this->colTypes[$filter->property];
					}
				}
				else {
					$qFilters[] = $filter->property . " " . $filter->comparator . " ?";
					$values[] = &$filter->value;
					$types[] = $this->colTypes[$filter->property];
				}
			}

			$query .= " WHERE " . join(" AND ", $qFilters);
		}

		$q = $this->connection->prepare($query);
		call_user_func_array([$q, "bind_param"], array_merge([implode($types)], $values));

		$result = $q->execute();
		$objects = [];
		if ($result) {
			$res = $q->get_result();
			while ($row = $res->fetch_assoc()) {
				$objects[] = $this->build($this->Model, $row);
			}
		}
			
		return $objects;
	}
}
