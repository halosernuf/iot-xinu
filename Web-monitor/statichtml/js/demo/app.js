angular.module('itaoke', [
    'ui.bootstrap',
    'ui.router',
    'restangular',
    'angular-table',
    'nsPopover',
    'ui.tree',
    'ngFileUpload',
    //'colorpicker.module',
    //'wysiwyg.module',
    'summernote'
]).config(function ($stateProvider, $urlRouterProvider, RestangularProvider, $locationProvider) {
    $stateProvider.state('demo', {
        url: '/demo',
        controller: 'DemoCtrl',
        templateUrl: 'partials/demo.html',
    });

    $urlRouterProvider.otherwise('/');
    $locationProvider.html5Mode(true);
}).run(function ($http, $rootScope) {
    
});
