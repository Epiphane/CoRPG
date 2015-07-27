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

	public $_new = true;

	public static function build($assoc) {
		$m = get_called_class();
		$model = new $m();

		foreach($assoc as $col => $val) {
			$model->$col = $val;
		}

		// Create primary key if not exists
		$pKey = self::getPrimaryKey($m);
		if (!$model->$pKey) {
			$model->$pKey = $model->createPrimaryKey();
		}

		return $model;
	}

	public static function getPrimaryKey($m) {
		$key = $m::$pKey ?: $m::$const_columns[0];
		if (!$key) {
			reset($m::$columns);
			$key = key($m::$columns);
		}
		return $key;
	}

	public function createPrimaryKey() {
		return mt_rand(1000000, 9999999);
	}

	public function save() {
		$model = get_called_class();
		$dao = new \Data\DAO($model);
	
		if ($this->_new) {
			return $dao->create($this);
		}
		else {
			return $dao->update($this, $model);
		}
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
