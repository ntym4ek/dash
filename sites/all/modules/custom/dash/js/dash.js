(function ($) {
  Drupal.behaviors.dash = {
    attach: function (context, settings) {

      $(".ajax-load").each(function() {
        var content = $(this).find(".row");
        if (!content.length) {
          var wrapper = $(this);
          var module = $(this).closest(".charts").data("module");
          var func = $(this).data("func");
          var periodFrom = $(this).closest(".charts").data("period-from");
          var periodTo = $(this).closest(".charts").data("period-to");
          var delay = $(this).data("delay");
          if (module && func) {
            setTimeout(function() {
              $.ajax({
                url: "/ajax-load?module=" + module + "&func=" + func + "&period_from=" + periodFrom + "&period_to=" + periodTo,
                success: function (data) {
                  wrapper.html(data);
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
