
function DashboardController($scope, $http, $timeout) {
    $scope.patients = [];
    $scope.refreshed = new Date(0);
    $scope.errorOccurred = false;
    $scope.result_class = function(value, min, max) {
        if (!min && !max) {
            return 'info'
        }
        if (value < min || value > max) {
            return 'danger'
        }
        var offset = 0.25 * (max - min)
        if (value < (min + offset) || value > (max - offset)) {
            return 'warning';
        }
        return 'success';
    }

    function poll() {
        var POLL_PERIOD = 5000;
        $http.get('cgi-bin/monitoring')
            .success(function(data) {
                $scope.patients = data.patients;
                $scope.refreshed = new Date();
                $scope.errorOccurred = false;
                preprocessPatientsData();
                analyzePatientsData();
                $timeout(poll, POLL_PERIOD);
            })
            .error(function(data, status) {
                $scope.errorOccurred = true;
                $timeout(poll, POLL_PERIOD);
            });
    }

    function analyzePatientsData() {
        var warningSound = document.getElementById('warningSound');
        if ($scope.patients.some(function(p) {
                return p.inDanger
            })) {
            warningSound.play();
        } else {
            warningSound.pause();
        }
    }

    function preprocessPatientsData() {
        $scope.patients.forEach(function(p) {
            setInDangerFlag(p);
            setLastUpdated(p);
        });
        console.log($scope.patients);
    }

    function setInDangerFlag(p){
      if (p.measurements.some(function(m) {
              return (m.min && (m.value < m.min)) || (m.max && (m.value > m.max))
          })) {
          p.inDanger = true;
      } else {
          p.inDanger = false;
      }
    }

    function setLastUpdated(p){
      lastUpdatedTimestamp = Math.min.apply(Math, p.measurements.map(function(o) {
          var parts = o.datetime.match(/(\d+)/g);
          return Date.UTC(parts[0], parts[1] - 1, parts[2], parts[3], parts[4], parts[5])
      }));
      if (lastUpdatedTimestamp != Infinity) {
          p.lastUpdated = moment(lastUpdatedTimestamp).fromNow();
      } else {
          p.lastUpdated = null;
      }
    }

    poll();
}
angular.module('SZOP', [])
    .controller('Dashboard', DashboardController);
