<?

namespace Data;

class DB
{
	// Database connection
	private static $host = "thomassteinke.db";
	private static $username = "thomassteink";
	private static $password = "2bORNOT2b";
	private static $database = "corpg";

	public static function getConnection() {
		$connect = mysqli_connect(self::$host, self::$username, self::$password, self::$database);

		if (mysqli_connect_errno()) {
			throw new \Exception("Failed to connect to MySQL: " . mysqli_connect_error());
		}

		return $connect;
	}
}
