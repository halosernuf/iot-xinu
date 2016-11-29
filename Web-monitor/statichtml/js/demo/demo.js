angular.module('itaoke').controller('DemoCtrl', function ($scope,$http,$filter) {
	//$scope.data = {message: "Hello"};
	
	var address="http://192.168.1.101:8080";
	$scope.viewtmp=false;
	$scope.viewdht=false;
	var getBBB={
		method: 'GET',
		url: address+'/helloRestful/hello/BBB',
	};
	$http(getBBB).then(function(res){
		$scope.bbb=res.data;
		console.log($scope.bbb);
	})
	$scope.updateB=function(){
		var getBBB={
			method: 'GET',
			url: address+'/helloRestful/hello/BBB',
		};
		$http(getBBB).then(function(res){
			$scope.bbb=res.data;
			console.log($scope.bbb);
			//getdevice();
		})
	}
	$scope.updateD=function(){
		console.log('update');
		var getBook={
		        method: 'GET',
		        url: address+'/helloRestful/hello/devices',
		}
			       
		 
		$http(getBook).then(function (res) {
			console.log(res.data);
			$scope.data=res.data;
			$scope.led=new Array();
			$scope.tmp=new Array();
			$scope.dht=new Array();
			for(var i=0;i<$scope.data.length;i++){
				console.log($scope.data[i].type);
				if($scope.data[i].type.match('LED')){
					$scope.led.push($scope.data[i]);
				}
				if($scope.data[i].type.match('TMP')){
					$scope.tmp.push($scope.data[i]);
					$scope.temp=$scope.data[i];
					var date=new Date();
					 $scope.temp.year=date.getYear();
					 $scope.temp.month=date.getMonth();
					 $scope.temp.day=date.getDay();
					 $scope.temp.hours=date.getHours();
					 $scope.temp.minutes=date.getMinutes();
					 $scope.temp.seconds=date.getSeconds();
					if(localStorage['recenttmp']){
          
				          var arrayObj=JSON.parse(localStorage['recenttmp']);
				          // if(arrayObj.length>=8){
				          //   arrayObj.shift();
				          // }
				          arrayObj.push( $scope.data[i]);
				          localStorage['recenttmp']= JSON.stringify(arrayObj);
				        }
				        else{
				          var arrayObj = new Array();
				          arrayObj.push($scope.data[i]);
				          localStorage['recenttmp']= JSON.stringify(arrayObj);
				          
				        }
					}
				if($scope.data[i].type.match('DHT')){
					$scope.temp=$scope.data[i];
					//$scope.temp.date=$filter("date")(new Date(), "dd/MM/yyyy HH:mm:ss");
					 $scope.temp.date=new Date();           
					 var date=new Date();
					 $scope.temp.year=date.getYear();
					 $scope.temp.month=date.getMonth();
					 $scope.temp.day=date.getDay();
					 $scope.temp.hours=date.getHours();
					 $scope.temp.minutes=date.getMinutes();
					 $scope.temp.seconds=date.getSeconds();
					if(localStorage['recentdht']){
          
				          var arrayObj=JSON.parse(localStorage['recentdht']);
				          // if(arrayObj.length>=8){
				          //   arrayObj.shift();
				          // }
				          arrayObj.push( $scope.temp);
				          localStorage['recentdht']= JSON.stringify(arrayObj);
				        }
				        else{
				          var arrayObj = new Array();
				          arrayObj.push($scope.temp);
				          localStorage['recentdht']= JSON.stringify(arrayObj);
				          
				        }
					

					$scope.dht.push($scope.data[i]);
				}
			}
			console.log(res);
		});
	}
	/*var data={
		"operation":1,
		"ip":"192.168.1.20",
		"id":"12"
	};
	var getBook={
		        method: 'POST',
		        url: 'http://192.168.1.21:8080/helloRestful/hello/LED',
		        data:JSON.stringify(data)
	}; 
	$http(getBook).then(function(res){
		$scope.data=res;
		console.log(res);
	});*/
	$scope.closeLED=function(ip,id){
		var data={
			"operation":"0",
			"ip":ip,
			"id":id
		};
		var Turnoff={
			        method: 'POST',
			        url: address+'/helloRestful/hello/LED',
			        data:JSON.stringify(data)
		}; 
		$http(Turnoff).then(function(res){

			var result=res.data.res;
			if(result=='OK'){
				for(var i=0;i<$scope.led.length;i++){
					if(ip==$scope.led[i].ip&&id==$scope.led[i].id){
						console.log($scope.led[i].status);
						$scope.led[i].status=false;
					}
				}	
			}
			else{
				alert('Failed');
			}
					console.log(res);
		});
		
	};
	$scope.openLED=function(ip,id){
		var data={
			"operation":"1",
			"ip":ip,
			"id":id
		};
		var Turnon={
			        method: 'POST',
			        url: address+'/helloRestful/hello/LED',
			        data:JSON.stringify(data)
		}; 
		$http(Turnon).then(function(res){
			var result=res.data.res;
			if(result=='OK'){
				for(var i=0;i<$scope.led.length;i++){
					if(ip==$scope.led[i].ip&&id==$scope.led[i].id){
						console.log($scope.led[i].status);
						$scope.led[i].status=true;
					}
				}	
			}
			else{
				alert('Failed');
			}
			console.log(res);
		});
	};
	$scope.viewRecentTMP=function(ip,id){
		$scope.viewtmp=true;
    	$scope.tmpdata=new Array();
    	var alldata=JSON.parse(localStorage['recenttmp']);
    	for(var i=0;i<alldata.length;i++){
    		if(alldata[i].ip==ip&&alldata[i].id==id){
    			$scope.tmpdata.push(alldata[i]);
    		}
    	}
    	//$scope.tmpdata=alldata;
    	console.log(alldata);
    	console.log($scope.tmpdata);
    	google.charts.load('current', {packages: ['corechart', 'line']});
		google.charts.setOnLoadCallback(drawCrosshairs);
	}
	$scope.viewRecentDHT=function(ip,id){
		$scope.viewdht=true;
		$scope.dhtdata=new Array();
		var alldata=JSON.parse(localStorage['recentdht']);
		for(var i=0;i<alldata.length;i++){
			if(alldata[i].ip==ip&&alldata[i].id==id){
				$scope.dhtdata.push(alldata[i]);
			}
		}
		//$scope.dhtdata=alldata;
		google.charts.load('current', {packages: ['corechart', 'line']});
		google.charts.setOnLoadCallback(drawCrosshairs1);
	}
function drawCrosshairs1() {
      var data = new google.visualization.DataTable();
      data.addColumn('datetime', 'X');
      data.addColumn('number', 'DHT11 Tempature');
      data.addColumn('number','DHT11 Humidity');
      console.log($scope.dhtdata);

      for(var i=0;i<$scope.dhtdata.length;i++){
      	data.addRows([[new Date($scope.dhtdata[i].year,$scope.dhtdata[i].month,$scope.dhtdata[i].day,$scope.dhtdata[i].hours,$scope.dhtdata[i].minutes,$scope.dhtdata[i].seconds),$scope.dhtdata[i].tempature,$scope.dhtdata[i].humidity]]);
      }

      /*data.addRows([
        [0, 0],    [1, 10, 5],   [2, 23, 15],  [3, 17, 9],   [4, 18, 10],  [5, 9, 5],
        [6, 11, 3],   [7, 27, 19],  [8, 33, 25],  [9, 40, 32],  [10, 32, 24], [11, 35, 27],
        [12, 30, 22], [13, 40, 32], [14, 42, 34], [15, 47, 39], [16, 44, 36], [17, 48, 40],
        [18, 52, 44], [19, 54, 46], [20, 42, 34], [21, 55, 47], [22, 56, 48], [23, 57, 49],
        [24, 60, 52], [25, 50, 42], [26, 52, 44], [27, 51, 43], [28, 49, 41], [29, 53, 45],
        [30, 55, 47], [31, 60, 52], [32, 61, 53], [33, 59, 51], [34, 62, 54], [35, 65, 57],
        [36, 62, 54], [37, 58, 50], [38, 55, 47], [39, 61, 53], [40, 64, 56], [41, 65, 57],
        [42, 63, 55], [43, 66, 58], [44, 67, 59], [45, 69, 61], [46, 69, 61], [47, 70, 62],
        [48, 72, 64], [49, 68, 60], [50, 66, 58], [51, 65, 57], [52, 67, 59], [53, 70, 62],
        [54, 71, 63], [55, 72, 64], [56, 73, 65], [57, 75, 67], [58, 70, 62], [59, 68, 60],
        [60, 64, 56], [61, 60, 52], [62, 65, 57], [63, 67, 59], [64, 68, 60], [65, 69, 61],
        [66, 70, 62], [67, 72, 64], [68, 75, 67], [69, 80, 72]
      ]);*/

      var options = {
        hAxis: {
          title: 'Time',
          format:'dd/MM/yyyy HH:mm:ss'
        },
        vAxis: {
          title: 'Tempature/Humidity'
        },
        colors: ['#a52714','#097138'],
        width:1225,
        crosshair: {
          color: '#000',
          trigger: 'selection'
        }
      };

      var chart = new google.visualization.LineChart(document.getElementById('chart_div dht'));

      chart.draw(data, options);
      chart.setSelection([{row: 38, column: 1}]);

    }
function drawCrosshairs() {
      var data = new google.visualization.DataTable();
      data.addColumn('datetime', 'X');
      data.addColumn('number', 'TMP36');
      for(var i=0;i<$scope.tmpdata.length;i++){
      	data.addRows([[new Date($scope.tmpdata[i].year,$scope.tmpdata[i].month,$scope.tmpdata[i].day,$scope.tmpdata[i].hours,$scope.tmpdata[i].minutes,$scope.tmpdata[i].seconds),$scope.tmpdata[i].tempature]]);
      }

      /*data.addRows([
        [0, 0],    [1, 10, 5],   [2, 23, 15],  [3, 17, 9],   [4, 18, 10],  [5, 9, 5],
        [6, 11, 3],   [7, 27, 19],  [8, 33, 25],  [9, 40, 32],  [10, 32, 24], [11, 35, 27],
        [12, 30, 22], [13, 40, 32], [14, 42, 34], [15, 47, 39], [16, 44, 36], [17, 48, 40],
        [18, 52, 44], [19, 54, 46], [20, 42, 34], [21, 55, 47], [22, 56, 48], [23, 57, 49],
        [24, 60, 52], [25, 50, 42], [26, 52, 44], [27, 51, 43], [28, 49, 41], [29, 53, 45],
        [30, 55, 47], [31, 60, 52], [32, 61, 53], [33, 59, 51], [34, 62, 54], [35, 65, 57],
        [36, 62, 54], [37, 58, 50], [38, 55, 47], [39, 61, 53], [40, 64, 56], [41, 65, 57],
        [42, 63, 55], [43, 66, 58], [44, 67, 59], [45, 69, 61], [46, 69, 61], [47, 70, 62],
        [48, 72, 64], [49, 68, 60], [50, 66, 58], [51, 65, 57], [52, 67, 59], [53, 70, 62],
        [54, 71, 63], [55, 72, 64], [56, 73, 65], [57, 75, 67], [58, 70, 62], [59, 68, 60],
        [60, 64, 56], [61, 60, 52], [62, 65, 57], [63, 67, 59], [64, 68, 60], [65, 69, 61],
        [66, 70, 62], [67, 72, 64], [68, 75, 67], [69, 80, 72]
      ]);*/

      var options = {
        hAxis: {
          title: 'Time',
          format:'dd/MM/yyyy HH:mm:ss'
        },
        vAxis: {
          title: 'Tempature'
        },
        colors: ['#a52714'],
        width:1225,
        crosshair: {
          color: '#000',
          trigger: 'selection'
        }
      };

      var chart = new google.visualization.LineChart(document.getElementById('chart_div tmp'));

      chart.draw(data, options);
      chart.setSelection([{row: 38, column: 1}]);

    }
	/*$scope.width = 600;
    $scope.height = 350;
    $scope.yAxis = 'Tempature';
    $scope.xAxis = 'Time';*/
});