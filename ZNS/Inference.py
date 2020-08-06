from typing import List

from ExpertSystem.Business.UserFramework import IInference, ActionBaseCaller
from ExpertSystem.Structure.Enums import LogicalOperator
from ExpertSystem.Structure.Enums import Operator
from ExpertSystem.Structure.RuleBase import Rule, Fact, ExpressionNode, Expression


class Inference(IInference):
    """
    | User definition of the inference. You can define here you inference method (forward or backward).
      You can have here as many functions as you want, but you must implement interfere with same signature

    |
    | `def interfere(self, knowledge_base: List[Fact], rules: List[Rule], action_base: ActionBase):`
    |

    | Method `interfere` will be called each turn or manually with `Inference` button.
    | Class have no class parameters, you can use only inference parameters

    """
    knowledge_base: List[Fact]
    action_base: ActionBaseCaller


    def infere(self, knowledge_base: List[Fact], rules: List[Rule], action_base: ActionBaseCaller) -> None:
        """
        User defined inference

        :param knowledge_base: - list of Fact classes defined in  KnowledgeBase.create_knowledge_base()
        :param rules:  - list of rules trees defined in rules file.
        :param action_base: - instance of ActionBaseCaller for executing conclusions
        
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!    TODO: Write implementation of your inference mechanism definition HERE    !!
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        """
        self.knowledge_base = knowledge_base
        self.action_base = action_base
        self.fuz_con = {}
        self.tv = 0
        self.bv = 0
        self.lv = 0
        self.mp = 0
        self.hp = 0
        self.fuz_val_set = set()
        self.defyz_res = {}

        fuzzy_list = []

        for rule in rules:
            if rule.conclusion.value.value is not None:
                fuzzy_evaluation = self.fuzzy_evaluation(rule.condition)
                rule_result = rule.conclusion.value.value
                conclusion_name = rule.conclusion.value.name
                fuzzy_list.append((conclusion_name, rule_result, fuzzy_evaluation))

        self.defuzzification(fuzzy_list)
            
        for rule in rules:
            condition = False
            if rule.conclusion.value.value is None:
                condition = self.rule_evaluation(rule.condition)
                if condition:
                    self.conclusion_evaluation(rule.conclusion)



    def rule_evaluation(self, root_node: ExpressionNode) -> bool:
        """
        Example of rule tree evaluation. This implementation did not check comparision operators and uncertainty.
        For usage in inference extend this function

        :param root_node: root node of the rule tree
        :return: True if rules is satisfiable, False in case of not satisfiable or missing Facts
        """
        if root_node.operator == LogicalOperator.AND:
            return self.rule_evaluation(root_node.left) and self.rule_evaluation(root_node.right)

        elif root_node.operator == LogicalOperator.OR:
            return self.rule_evaluation(root_node.left) or self.rule_evaluation(root_node.right)

        elif isinstance(root_node.value, Expression):
            try:
                if root_node.value.name in self.fuz_val_set:
                    print('tut')
                    print(self.defyz_res[root_node.value.name])
                    return self.defyz_res[root_node.value.name]
                return self.knowledge_base[self.knowledge_base.index(root_node.value.name)](*root_node.value.args)
            except ValueError:
                return False

        else:
            return bool(root_node.value)

    def fuzzy_evaluation(self, root_node: ExpressionNode):
        if root_node.operator == LogicalOperator.AND:
            if root_node.left.value.value == 'closely':
                self.c1 = self.knowledge_base[self.knowledge_base.index(root_node.left.value.name)](*root_node.left.value.args)[0]
            elif root_node.left.value.value == 'middle':
                self.c1 = self.knowledge_base[self.knowledge_base.index(root_node.left.value.name)](*root_node.left.value.args)[1]
            elif root_node.left.value.value == 'free':
                self.c1 = self.knowledge_base[self.knowledge_base.index(root_node.left.value.name)](*root_node.left.value.args)[2]
            if root_node.right.value.value == 'begin':
                self.c2 = self.knowledge_base[self.knowledge_base.index(root_node.right.value.name)](*root_node.right.value.args)[0]
            elif root_node.right.value.value == 'middle':
                self.c2 = self.knowledge_base[self.knowledge_base.index(root_node.right.value.name)](*root_node.right.value.args)[1]
            elif root_node.right.value.value == 'end':
                self.c2 = self.knowledge_base[self.knowledge_base.index(root_node.right.value.name)](*root_node.right.value.args)[2]
            if self.c1 == None or self.c2 == None: return 1
            return min(self.c1, self.c2)
            
        elif isinstance(root_node.value, Expression):
            try:
                closely, mid, lot = self.knowledge_base[self.knowledge_base.index(root_node.value.name)](*root_node.value.args)
                if root_node.value.value == 'closely':
                    return self.knowledge_base[self.knowledge_base.index(root_node.value.name)](*root_node.value.args)[0]
                elif root_node.value.value == 'middle':
                    return self.knowledge_base[self.knowledge_base.index(root_node.value.name)](*root_node.value.args)[1]
                elif root_node.value.value == 'free':
                    return self.knowledge_base[self.knowledge_base.index(root_node.value.name)](*root_node.value.args)[2]
            except ValueError:
                return False
        else:
            return bool(root_node.value)


    def defuzzification(self, fuzzy_list):
        self.tv = 0
        self.bv = 0
        self.lv = 0
        self.mp = 0
        self.hp = 0
        for fuz_res in fuzzy_list:
            if fuz_res[0] not in self.fuz_val_set:
                self.fuz_con[fuz_res[0]] = (fuz_res[2], fuz_res[1])
                self.fuz_val_set.add(fuz_res[0])
            else:
                if self.fuz_con[fuz_res[0]][0] < fuz_res[2]:
                    self.fuz_con[fuz_res[0]] = (fuz_res[2], fuz_res[1])

        for fuzzy_values in self.fuz_val_set:
            if self.fuz_con[fuz_res[0]][1] == 'dont':
                self.lv = self.fuz_con[fuz_res[0]][0]
            elif self.fuz_con[fuz_res[0]][1] == 'should':
                self.mp = self.fuz_con[fuz_res[0]][0]
            else:
                self.hp = self.fuz_con[fuz_res[0]][0]
            for i in range(10):
                if 0 <= i <= 3:
                    self.tv += i * self.lv
                    self.bv += self.lv
                elif 4 <= i <= 6:
                    self.tv += i * self.mp
                    self.bv += self.mp
                else:
                    self.tv += i * self.hp
                    self.bv += self.hp
            self.defyz_res[fuzzy_values] = \
                self.tv / (1 if self.bv == 0 else self.bv) >= 5


    def conclusion_evaluation(self, root_node: ExpressionNode):
        if self.action_base.has_method(root_node.value):
            self.action_base.call(root_node.value)
