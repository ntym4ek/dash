(function ($) {
  Drupal.behaviors.dash = {
    attach: function (context, settings) {

      // - ajax обработчик блоков графиков
      var module = $(".charts").data("module");
      var periodFrom = $(".charts").data("period-from");
      var periodTo = $(".charts").data("period-to");
      var periodEnd = $(".charts").data("period-end");

      $(".ajax-load").once(function() {
        var content = $(this).find(".row");
        if (!content.length) {
          var wrapper = $(this);
          var func = $(this).data("func");
          var delay = $(this).data("delay");
          if (module && func) {
            setTimeout(function() {
              $.ajax({
                url: "/ajax-load?module=" + module + "&func=" + func + "&period_from=" + periodFrom + "&period_to=" + periodTo + "&period_end=" + periodEnd,
                success: function (data) {
                  // wrapper.html(data);
                  var newNodes = wrapper.html(data);
                  Drupal.attachBehaviors(newNodes);
                }
              });
            }, delay);
          }
        }
      });
    }
  };
})(jQuery);
