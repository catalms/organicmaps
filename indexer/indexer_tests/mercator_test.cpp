#include "../../base/SRC_FIRST.hpp"

#include "../../testing/testing.hpp"

#include "../../indexer/mercator.hpp"

#include "../../base/math.hpp"
#include "../../base/macros.hpp"
#include "../../base/logging.hpp"


UNIT_TEST(Mercator_Grid)
{
  double const eps = 0.0000001;
  for (int lat = -85; lat <= 85; ++lat)
  {
    for (int lon = -180; lon <= 180; ++lon)
    {
      double const x = MercatorBounds::LonToX(lon);
      double const y = MercatorBounds::LatToY(lat);
      double const lat1 = MercatorBounds::YToLat(y);
      double const lon1 = MercatorBounds::XToLon(x);

      // Normal assumption for any projection.
      TEST_ALMOST_EQUAL(static_cast<double>(lat), lat1, ());
      TEST_ALMOST_EQUAL(static_cast<double>(lon), lon1, ());

      // x is actually lon unmodified.
      TEST_ALMOST_EQUAL(x, static_cast<double>(lon), ());

      if (lat == 0)
      {
        // TODO: Investigate, how to make Mercator transform more precise.
        // Error is to large for TEST_ALMOST_EQUAL(y, 0.0, ());
        TEST_LESS(fabs(y), eps, (lat, y, lat1));
      }
    }
  }
}

UNIT_TEST(Mercator_DirectInferseF)
{
  double const eps = 0.0000001;
  double lon = 63.45421;
  double x = MercatorBounds::LonToX(lon);
  double lon1 = MercatorBounds::XToLon(x);
  TEST_LESS(fabs(lon - lon1), eps, ("Too big round error"));
  double lat = 34.28754;
  double y = MercatorBounds::LatToY(lat);
  double lat1 = MercatorBounds::YToLat(y);
  TEST_LESS(fabs(lat - lat1), eps, ("Too big round error"));
  TEST_LESS(fabs(MercatorBounds::maxX - MercatorBounds::maxY), eps, ("Non-square maxX and maxY"));
  TEST_LESS(fabs(MercatorBounds::minX - MercatorBounds::minY), eps, ("Non-square minX and minY"));
}

UNIT_TEST(Mercator_ErrorToRadius)
{
  double const points[] = { -85.0, -45.0, -10.0, -1.0, -0.003, 0.0, 0.003, 1.0, 10.0, 45.0, 85.0 };

  double const error1 = 1.0;    // 1 metre
  double const error10 = 10.0;  // 10 metres

  for (size_t i = 0; i < ARRAY_SIZE(points); ++i)
  {
    for (size_t j = 0; j < ARRAY_SIZE(points); ++j)
    {
      double const lon = points[i];
      double const lat = points[j];
      m2::PointD const mercPoint(MercatorBounds::LonToX(lon), MercatorBounds::LatToY(lat));

      m2::RectD radius1 = MercatorBounds::MetresToXY(lon, lat, error1);
      TEST(radius1.IsPointInside(mercPoint), (lat, lon));

      m2::RectD radius10 = MercatorBounds::MetresToXY(lon, lat, error10);
      TEST(radius10.IsPointInside(mercPoint), (lat, lon));

      m2::RectD radius10Added = radius10;
      radius10Added.Add(radius1);
      TEST_EQUAL(radius10, radius10Added, (lat, lon));

      TEST(radius10.IsPointInside(radius1.LeftTop()), (lat, lon));
      TEST(radius10.IsPointInside(radius1.LeftBottom()), (lat, lon));
      TEST(radius10.IsPointInside(radius1.RightTop()), (lat, lon));
      TEST(radius10.IsPointInside(radius1.RightBottom()), (lat, lon));
    }
  }
}

UNIT_TEST(Mercator_Sample1)
{
  LOG(LINFO, (MercatorBounds::XToLon(27.531491200000001385),
              MercatorBounds::YToLat(64.392864299248202542)));
}
