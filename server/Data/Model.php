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
	public static $columns = [];

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
