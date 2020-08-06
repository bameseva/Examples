from typing import List
from OrodaelTurrim.Business.Interface.Player import PlayerTag
from OrodaelTurrim.Business.Proxy import MapProxy, GameObjectProxy, GameUncertaintyProxy
from ExpertSystem.Business.UserFramework import IKnowledgeBase
from ExpertSystem.Structure.RuleBase import Fact
from OrodaelTurrim.Structure.Enums import TerrainType, AttributeType, EffectType, GameRole
from OrodaelTurrim.Structure.Position import OffsetPosition, CubicPosition, AxialPosition


class KnowledgeBase(IKnowledgeBase):
    """
    Class for defining known facts based on Proxy information. You can transform here any information from
    proxy to better format of Facts. Important is method `create_knowledge_base()`. Return value of this method
    will be passed to `Inference.interfere`. It is recommended to use Fact class but you can use another type.

    |
    |
    | Class provides attributes:

    - **map_proxy [MapProxy]** - Proxy for access to map information
    - **game_object_proxy [GameObjectProxy]** - Proxy for access to all game object information
    - **uncertainty_proxy [UncertaintyProxy]** - Proxy for access to all uncertainty information in game
    - **player [PlayerTag]** - class that serve as instance of user player for identification in proxy methods

    """
    map_proxy: MapProxy
    game_object_proxy: GameObjectProxy
    game_uncertainty_proxy: GameUncertaintyProxy
    player: PlayerTag


    def __init__(self, map_proxy: MapProxy, game_object_proxy: GameObjectProxy,
                 game_uncertainty_proxy: GameUncertaintyProxy, player: PlayerTag):
        """
        You can add some code to __init__ function, but don't change the signature. You cannot initialize
        KnowledgeBase class manually so, it is make no sense to change signature.
        """
        super().__init__(map_proxy, game_object_proxy, game_uncertainty_proxy, player)


    def create_knowledge_base(self) -> List[Fact]:
        """
        Method for create user knowledge base. You can also have other class methods, but entry point must be this
        function. Don't change the signature of the method, you can change return value, but it is not recommended.

        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!  TODO: Write implementation of your knowledge base definition HERE   !!
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        """

        facts = []

        if not self.map_proxy.player_have_base(self.player):
            facts.append(Fact('player_dont_have_base'))

        facts.append(Fact('surrounded', eval_function=self.all_fuzzy_surrounded, data=self.all_fuzzy_surrounded))
        facts.append(Fact('game_period', eval_function=self.all_fuzzy_level_number, data=self.all_fuzzy_level_number))

        

        facts.append(Fact('tiles_for_protection', eval_function=self.tiles_for_protection, data=self.tiles_for_protection))


        return facts


    def tiles_for_protection(self):
        res = []
        tiles = self.map_proxy.get_player_visible_tiles()
        border_tiles = self.map_proxy.get_border_tiles()
        position_of_base = None
        for i in self.map_proxy.get_bases_positions():
            position_of_base = i
        base_neighbours = position_of_base.get_all_neighbours()
        for tile in base_neighbours:
            occupied = self.map_proxy.is_position_occupied(tile)
            resources = self.game_object_proxy.get_resources(self.player)
            if not occupied and tile not in border_tiles and tile in tiles:
                res.append(tile)

        return res


    def free_tile_to_attack(self):
        tiles = self.map_proxy.get_player_visible_tiles()
        border_tiles = self.map_proxy.get_border_tiles()

        for tile in self.actual_tiles_for_attack():
            occupied = self.map_proxy.is_position_occupied(tile)
            resources = self.game_object_proxy.get_resources(self.player)
            if not occupied and tile not in border_tiles and tile in tiles and resources > 17:
                return tile
        return None

    
    def all_fuzzy_level_number(self):
        return self.fuzzy_level_number('begin'), self.fuzzy_level_number('middle'), self.fuzzy_level_number('end')


    def fuzzy_level_number(self, value):
        if value == 'begin':
            return self.fuzzy_calc(self.game_object_proxy.get_current_round(), 0, 0, 4, 5)

        if value == 'middle':
            return self.fuzzy_calc(self.game_object_proxy.get_current_round(), 4, 5, 10, 11)

        if value == 'end':
            return self.fuzzy_calc(self.game_object_proxy.get_current_round(), 10, 11, 16, 25)

    def fuzzy_calc(self, x, x1, x2, x3, x4):
        if x1 == x2:
            if x <= x3:
                return 1
            if x >= x4:
                return 0
            if x3 < x < x4:
                return (x - x4) / (x3 - x4)

        if x3 == x4:
            if x <= x1:
                return 0
            if x >= x2:
                return 1
            if x1 < x < x2:
                return (x - x1) / (x2 - a)

        return max(min((x - x1) / (x2 - x1), 1, (x4 - x) / (x4 - x3)), 0)


    def all_fuzzy_surrounded(self):
        return self.fuzzy_surrounded('free'), self.fuzzy_surrounded('middle'), self.fuzzy_surrounded('closely')


    def fuzzy_surrounded(self, value):
        if value == 'free':
            return self.fuzzy_calc(self.get_number_of_neighbours(), 0, 0, 1, 2)

        if value == 'middle':
            return self.fuzzy_calc(self.get_number_of_neighbours(), 1, 2, 3, 4)

        if value == 'closely':
            return self.fuzzy_calc(self.get_number_of_neighbours(), 3, 4, 5, 6)


    def get_number_of_neighbours(self):
        return len([x for x in OffsetPosition(0, 0).get_all_neighbours() if self.map_proxy.is_position_occupied(x)])


    def actual_tiles_for_attack(self):
        return [
            OffsetPosition(0, 2),
            OffsetPosition(0, -2),
            OffsetPosition(2, 0),
            OffsetPosition(-2, 0),
            OffsetPosition(2, 1),
            OffsetPosition(-2, -1),
            OffsetPosition(1, 2),
            OffsetPosition(-1, -2),
            OffsetPosition(-1, 2),
            OffsetPosition(1, -2),
            OffsetPosition(-2, 1),
            OffsetPosition(2, -1),
            OffsetPosition(1, 1),
            OffsetPosition(-1, 1)
        ]

