<?php
/*
 * Model class file
 *
 * @author Thomas Steinke
 */
namespace Data;

use \Data\DAO;

class Model
{
	public static $tableName;
	public static $pKey = null;
	public static $columns = [];
	public static $const_columns = [];

	public static function build($assoc) {
		$m = get_called_class();
		$model = new $m();

		foreach($assoc as $col => $val) {
			$model->$col = $val;
		}

		return $model;
	}

	public function read() {
		return $this;
	}

	public function update($attrs) {
		foreach(self::$const_columns as $key) {
			unset($attrs[$key]);
		}

		$dao = new \Data\DAO(get_called_class());

		return $dao->update($this, $attrs);
	}

	public static function find($request) {
		$dao = new \Data\DAO(get_called_class());

		return $dao->find($request);
	}

	public static function findOne($request) {
		$dao = new \Data\DAO(get_called_class());

		$game_object = $dao->findOne($request);

		return $game_object;
	}
}
